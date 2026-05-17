#include "trampoline.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096
#define JUMP_SIZE 6

// Returns x86 register encoding (0-7) if addr is __x86.get_pc_thunk.*, else -1.
static int get_pc_thunk_reg(void *addr) {
    uint8_t *p = addr;
    if (p[0] == 0xF3 && p[1] == 0x0F && p[2] == 0x1E && p[3] == 0xFB)
        p += 4; // skip endbr32
    // mov (%esp), %reg  (8B [00 reg 100] 24)  +  ret
    if (p[0] == 0x8B && (p[1] & 0xC7) == 0x04 && p[2] == 0x24 && p[3] == 0xC3)
        return (p[1] >> 3) & 0x07;
    return -1;
}

static void relocate_instruction(
    ZydisDisassembledInstruction *instr,
    uint8_t *buffer, size_t *offset,
    uint64_t new_base)
{
    // call __x86.get_pc_thunk.* sets a register to the return address so PIC
    // code can find the GOT. Relocated to the trampoline the return address
    // would be wrong, so replace it with an immediate mov of the original value.
    if (instr->info.mnemonic == ZYDIS_MNEMONIC_CALL) {
        ZydisDecodedOperand *op = &instr->operands[0];
        if (op->type == ZYDIS_OPERAND_TYPE_IMMEDIATE && op->imm.is_relative) {
            ZyanU64 abs_target;
            ZydisCalcAbsoluteAddress(&instr->info, op, instr->runtime_address, &abs_target);
            int reg = get_pc_thunk_reg((void *)(uintptr_t)abs_target);
            if (reg >= 0) {
                uint32_t next_addr = (uint32_t)(instr->runtime_address + instr->info.length);
                buffer[*offset] = 0xB8 + reg; // MOV r32, imm32
                memcpy(buffer + *offset + 1, &next_addr, 4);
                *offset += 5;
                return;
            }
        }
    }

    ZydisEncoderRequest req;
    ZydisEncoderDecodedInstructionToEncoderRequest(
        &instr->info, instr->operands, instr->info.operand_count_visible, &req);

    for (int i = 0; i < instr->info.operand_count_visible; i++) {
        ZydisDecodedOperand *op = &instr->operands[i];

        if (op->type == ZYDIS_OPERAND_TYPE_MEMORY &&
            op->mem.base == ZYDIS_REGISTER_RIP) {
            ZyanU64 abs_addr;
            ZydisCalcAbsoluteAddress(&instr->info, op, instr->runtime_address, &abs_addr);
            req.operands[i].mem.displacement = (ZyanI64)abs_addr;
        }

        if (op->type == ZYDIS_OPERAND_TYPE_IMMEDIATE && op->imm.is_relative) {
            ZyanU64 abs_addr;
            ZydisCalcAbsoluteAddress(&instr->info, op, instr->runtime_address, &abs_addr);
            req.operands[i].imm.u = abs_addr;
            req.branch_width = ZYDIS_BRANCH_WIDTH_NONE;
        }
    }

    ZyanUSize encoded_len = 15;
    ZydisEncoderEncodeInstructionAbsolute(&req, buffer + *offset, &encoded_len,
        new_base + *offset);
    *offset += encoded_len;
}

void *trampoline_install(void *func, void *detour) {
    void *tramp = mmap(NULL,
        PAGE_SIZE,
        PROT_READ | PROT_WRITE | PROT_EXEC,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );

    uint8_t shellcode[JUMP_SIZE] = {
        0x68, 0x00, 0x00, 0x00, 0x00, // PUSH imm32
        0xC3                          // RET
    };

    ZyanU8 *data = func;
    ZyanU64 runtime_address = (ZyanU64)(uintptr_t)func;

    ZyanUSize src_offset = 0;
    size_t tramp_offset = 0;
    ZydisDisassembledInstruction instruction = { 0 };

    while (ZYAN_SUCCESS(ZydisDisassembleIntel(
        ZYDIS_MACHINE_MODE_LEGACY_32,
        runtime_address,
        data + src_offset,
        16,
        &instruction
    ))) {
        relocate_instruction(&instruction, (uint8_t *)tramp, &tramp_offset, (uint64_t)(uintptr_t)tramp);
        src_offset += instruction.info.length;
        runtime_address += instruction.info.length;

        if (src_offset >= JUMP_SIZE) { break; }
    }

    void *func_continue = (uint8_t *)func + src_offset;
    *(uint32_t *)(&shellcode[1]) = (uint32_t)(uintptr_t)func_continue;
    memcpy((uint8_t *)tramp + tramp_offset, shellcode, JUMP_SIZE);

    mprotect(
        (void *)((uintptr_t)func - ((uintptr_t)func % PAGE_SIZE)),
        PAGE_SIZE * 2,
        PROT_EXEC | PROT_READ | PROT_WRITE
    );

    *(uint32_t *)(&shellcode[1]) = (uint32_t)(uintptr_t)detour;
    memcpy(func, shellcode, JUMP_SIZE);
    memset((uint8_t *)func + JUMP_SIZE, 0x90, src_offset - JUMP_SIZE);

    return tramp;
}
