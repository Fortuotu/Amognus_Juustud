
// Namespace: 
[DisallowMultipleComponent]
public class CustomNetworkTransform : InnerNetObject // TypeDefIndex: 1043
{
	// Fields
	private const float SEND_MOVEMENT_THRESHOLD = 0.0001;
	private const float REPLAY_POSITION_THRESHOLD = 0.003;
	private const float NEUTRAL_BAND_MODIFIER = 0.995;
	private const float SMOOTHING_BAND_MODIFIER = 0.5;
	private const float SMOOTHING_LERP_RATE = 3;
	private const int QUEUE_LENGTH_FOR_SNAPPING = 12;
	private const int QUEUE_LENGTH_FOR_SECOND_SNAP = 14;
	private const int QUEUE_THRESHOLD_FOR_SMOOTHING = 5;
	private const int SNAP_TO_SEQUENCE_ID_BUFFER = 2;
	private PlayerControl myPlayer; // 0x28
	private Rigidbody2D body; // 0x2C
	private Queue<Vector2> sendQueue; // 0x30
	private Queue<Vector2> incomingPosQueue; // 0x34
	[SerializeField]
	private float rubberbandModifier; // 0x38
	[SerializeField]
	private float idealSpeed; // 0x3C
	private bool isPaused; // 0x40
	private ushort lastSequenceId; // 0x42
	private Vector2 lastPosition; // 0x44
	private Vector2 lastPosSent; // 0x4C
	private Nullable<Vector2> tempSnapPosition; // 0x54
	private ITransformGhost debugPopPositions; // 0x60
	private ITransformGhost debugTargetPositions; // 0x64
	private ITransformGhost debugTruePositions; // 0x68
	private INetTransformLogger debugNetLogger; // 0x6C

	// Methods

	// RVA: 0x533580 Offset: 0x532980 VA: 0x10533580
	private void Awake() { }

	// RVA: 0x5346D0 Offset: 0x533AD0 VA: 0x105346D0
	public void OnDrawGizmos() { }

	// RVA: 0x534850 Offset: 0x533C50 VA: 0x10534850
	public void OnEnable() { }

	// RVA: 0x4A4780 Offset: 0x4A3B80 VA: 0x104A4780
	public void SetPaused(bool isPaused) { }

	// RVA: 0x5340D0 Offset: 0x5334D0 VA: 0x105340D0
	public void Halt() { }

	// RVA: 0x5348F0 Offset: 0x533CF0 VA: 0x105348F0
	public void RpcSnapTo(Vector2 position) { }

	// RVA: 0x5351F0 Offset: 0x5345F0 VA: 0x105351F0
	public void SnapTo(Vector2 position) { }

	// RVA: 0x5336A0 Offset: 0x532AA0 VA: 0x105336A0
	public void ClearPositionQueues() { }

	// RVA: 0x5342F0 Offset: 0x5336F0 VA: 0x105342F0
	private bool IsInMiddleOfAnimationThatMakesPlayerInvisible() { }

	// RVA: 0x534FC0 Offset: 0x5343C0 VA: 0x10534FC0
	private void SnapTo(Vector2 position, ushort minSid) { }

	// RVA: 0x533C80 Offset: 0x533080 VA: 0x10533C80
	private void FixedUpdate() { }

	// RVA: 0x5341D0 Offset: 0x5335D0 VA: 0x105341D0
	private bool HasMoved() { }

	// RVA: 0x534140 Offset: 0x533540 VA: 0x10534140 Slot: 7
	public override void HandleRpc(byte callId, MessageReader reader) { }

	// RVA: 0x533680 Offset: 0x532A80 VA: 0x10533680 Slot: 8
	public override void ClearOrDecrementDirt() { }

	// RVA: 0x5349F0 Offset: 0x533DF0 VA: 0x105349F0 Slot: 9
	public override bool Serialize(MessageWriter writer, bool initialState) { }

	// RVA: 0x533700 Offset: 0x532B00 VA: 0x10533700 Slot: 10
	public override void Deserialize(MessageReader reader, bool initialState) { }

	// RVA: 0x534340 Offset: 0x533740 VA: 0x10534340
	private void MoveTowardNextPoint() { }

	// RVA: 0x534E50 Offset: 0x534250 VA: 0x10534E50
	private void SkipExcessiveFrames() { }

	// RVA: 0x534DD0 Offset: 0x5341D0 VA: 0x10534DD0
	private bool ShouldExtendCurrentFrame(Vector2 nextPos, Vector2 currentPos) { }

	// RVA: 0x533AB0 Offset: 0x532EB0 VA: 0x10533AB0
	private bool DidPassPosition(Vector2 nextPos, Vector2 lastPos, Vector2 currentPos) { }

	// RVA: 0x534D20 Offset: 0x534120 VA: 0x10534D20
	private void SetMovementSmoothingModifier() { }

	// RVA: 0x535220 Offset: 0x534620 VA: 0x10535220
	public void .ctor() { }
}


// Namespace: 
public class PlayerControl : InnerNetObject // TypeDefIndex: 1461
{
	// Fields
	private const int MAX_NETOBJECT_INIT_ATTEMPTS = 60;
	private const float NETOBJECT_INIT_ATTEMPT_WAIT_SECONDS = 0.1;
	public byte PlayerId; // 0x28
	public string FriendCode; // 0x2C
	public string Puid; // 0x30
	public float MaxReportDistance; // 0x34
	public bool moveable; // 0x38
	public CosmeticsLayer cosmetics; // 0x3C
	[HideInInspector]
	public bool ForceKillTimerContinue; // 0x40
	[CompilerGenerated]
	private PlayerOutfitType <CurrentOutfitType>k__BackingField; // 0x44
	public bool inVent; // 0x48
	public bool walkingToVent; // 0x49
	public bool petting; // 0x4A
	public bool inMovingPlat; // 0x4B
	public bool onLadder; // 0x4C
	public bool protectedByGuardianThisRound; // 0x4D
	public bool shapeshifting; // 0x4E
	public bool waitingForShapeshiftResponse; // 0x4F
	public bool isKilling; // 0x50
	private float invisibilityAlpha; // 0x54
	private const float OFFSET_ANIM_CHARGE = -0.05;
	public static PlayerControl LocalPlayer; // 0x0
	private NetworkedPlayerInfo CachedPlayerData; // 0x58
	private int protectedByGuardianId; // 0x5C
	private float flashlightAngle; // 0x60
	private int shapeshiftTargetPlayerId; // 0x64
	private bool shouldAppearInvisible; // 0x68
	public bool isTrackingPlayer; // 0x69
	public PlayerControl trackedPlayer; // 0x6C
	public int trackedPlayerColorID; // 0x70
	public AudioSource FootSteps; // 0x74
	public AudioClip KillSfx; // 0x78
	public KillAnimation[] KillAnimations; // 0x7C
	[SerializeField]
	private float killTimer; // 0x80
	public int RemainingEmergencies; // 0x84
	public LightSource LightPrefab; // 0x88
	private LightSource lightSource; // 0x8C
	[HideInInspector]
	public Collider2D Collider; // 0x90
	[HideInInspector]
	public PlayerPhysics MyPhysics; // 0x94
	[HideInInspector]
	public CustomNetworkTransform NetTransform; // 0x98
	[SerializeField]
	private Collider2D clickKillCollider; // 0x9C
	public Vector3 defaultCosmeticsScale; // 0xA0
	public List<PlayerTask> myTasks; // 0xAC
	public List<RoleEffectAnimation> currentRoleAnimations; // 0xB0
	public GameObject TargetFlashlight; // 0xB4
	public bool isDummy; // 0xB8
	public bool notRealPlayer; // 0xB9
	private readonly Logger logger; // 0xBC
	private readonly List<IPlayerVisibleItem> visibilityItems; // 0xC0
	private Collider2D[] hitBuffer; // 0xC4
	private IUsable closest; // 0xC8
	private bool isNew; // 0xCC
	private bool hasBeenSerialized; // 0xCD
	private Rigidbody2D rigidbody2D; // 0xD0
	public static List<PlayerControl> AllPlayerControls; // 0x4
	private Dictionary<Collider2D, IUsable[]> cache; // 0xD4
	private List<IUsable> itemsInRange; // 0xD8
	private List<IUsable> newItemsInRange; // 0xDC
	private byte scannerCount; // 0xE0
	private bool roleAssigned; // 0xE1
	private int LastStartCounter; // 0xE4

	// Properties
	public bool CanMove { get; }
	public bool IsKillTimerEnabled { get; }
	public NetworkedPlayerInfo.PlayerOutfit CurrentOutfit { get; }
	public PlayerOutfitType CurrentOutfitType { get; set; }
	public float CalculatedAlpha { get; }
	public float FlashlightAngle { get; set; }
	public bool PhantomFadeActive { get; }
	public NetworkedPlayerInfo Data { get; }
	public bool Visible { get; set; }
	public PlayerBodyTypes BodyType { get; }

	// Methods

	// RVA: 0x5CC560 Offset: 0x5CB960 VA: 0x105CC560
	public bool get_CanMove() { }

	// RVA: 0x5CCA40 Offset: 0x5CBE40 VA: 0x105CCA40
	public bool get_IsKillTimerEnabled() { }

	// RVA: 0x5CC890 Offset: 0x5CBC90 VA: 0x105CC890
	public NetworkedPlayerInfo.PlayerOutfit get_CurrentOutfit() { }

	[CompilerGenerated]
	// RVA: 0x3C17E0 Offset: 0x3C0BE0 VA: 0x103C17E0
	public PlayerOutfitType get_CurrentOutfitType() { }

	[CompilerGenerated]
	// RVA: 0x3ADDD0 Offset: 0x3AD1D0 VA: 0x103ADDD0
	private void set_CurrentOutfitType(PlayerOutfitType value) { }

	// RVA: 0x5CC550 Offset: 0x5CB950 VA: 0x105CC550
	public float get_CalculatedAlpha() { }

	// RVA: 0x5CCA30 Offset: 0x5CBE30 VA: 0x105CCA30
	public float get_FlashlightAngle() { }

	// RVA: 0x5CCC50 Offset: 0x5CC050 VA: 0x105CCC50
	public void set_FlashlightAngle(float value) { }

	// RVA: 0x5CCC00 Offset: 0x5CC000 VA: 0x105CCC00
	public bool get_PhantomFadeActive() { }

	// RVA: 0x5CC970 Offset: 0x5CBD70 VA: 0x105CC970
	public NetworkedPlayerInfo get_Data() { }

	// RVA: 0x5C9E00 Offset: 0x5C9200 VA: 0x105C9E00
	public void SetKillTimer(float time) { }

	// RVA: 0x5CCC20 Offset: 0x5CC020 VA: 0x105CCC20
	public bool get_Visible() { }

	// RVA: 0x5CCC70 Offset: 0x5CC070 VA: 0x105CCC70
	public void set_Visible(bool value) { }

	// RVA: 0x5CC500 Offset: 0x5CB900 VA: 0x105CC500
	public PlayerBodyTypes get_BodyType() { }

	// RVA: 0x5BE8B0 Offset: 0x5BDCB0 VA: 0x105BE8B0
	private void Awake() { }

	// RVA: 0x5C5B90 Offset: 0x5C4F90 VA: 0x105C5B90
	private void OnEnable() { }

	// RVA: 0x5C5AB0 Offset: 0x5C4EB0 VA: 0x105C5AB0
	private void OnDisable() { }

	[IteratorStateMachine(typeof(PlayerControl.<AssertWithTimeout>d__81))]
	// RVA: 0x5BE850 Offset: 0x5BDC50 VA: 0x105BE850
	private IEnumerator AssertWithTimeout(Func<bool> assertion, Action onTimeout, float timeoutInSeconds) { }

	[IteratorStateMachine(typeof(PlayerControl.<Start>d__82))]
	// RVA: 0x5CB890 Offset: 0x5CAC90 VA: 0x105CB890
	private IEnumerator Start() { }

	[IteratorStateMachine(typeof(PlayerControl.<ClientInitialize>d__83))]
	// RVA: 0x5C06D0 Offset: 0x5BFAD0 VA: 0x105C06D0
	private IEnumerator ClientInitialize() { }

	// RVA: 0x5C5A30 Offset: 0x5C4E30 VA: 0x105C5A30 Slot: 6
	public override void OnDestroy() { }

	// RVA: 0x5C2520 Offset: 0x5C1920 VA: 0x105C2520
	private void FixedUpdate() { }

	// RVA: 0x5BE5E0 Offset: 0x5BD9E0 VA: 0x105BE5E0
	public void AnimateCustom(AnimationClip anim) { }

	// RVA: 0x5C5CB0 Offset: 0x5C50B0 VA: 0x105C5CB0
	public void OnGameStart() { }

	// RVA: 0x5C5C80 Offset: 0x5C5080 VA: 0x105C5C80
	public void OnGameEnd() { }

	// RVA: 0x5CC190 Offset: 0x5CB590 VA: 0x105CC190
	public void UseClosest() { }

	// RVA: 0x5C6750 Offset: 0x5C5B50 VA: 0x105C6750
	public void RegisterVisibilityItem(IPlayerVisibleItem obj) { }

	// RVA: 0x5CC140 Offset: 0x5CB540 VA: 0x105CC140
	public void UnregisterVisibilityItem(IPlayerVisibleItem obj) { }

	// RVA: 0x5CB9F0 Offset: 0x5CADF0 VA: 0x105CB9F0
	public void TryPet() { }

	// RVA: 0x5C6B80 Offset: 0x5C5F80 VA: 0x105C6B80
	public void ReportClosest() { }

	// RVA: 0x5C5E70 Offset: 0x5C5270 VA: 0x105C5E70
	public void PlayStepSound() { }

	// RVA: 0x5CA960 Offset: 0x5C9D60 VA: 0x105CA960
	private void SetScanner(bool on, byte cnt) { }

	// RVA: 0x5C3890 Offset: 0x5C2C90 VA: 0x105C3890
	public Vector2 GetTruePosition() { }

	// RVA: 0x5CABF0 Offset: 0x5C9FF0 VA: 0x105CABF0
	public void SetTasks(List<NetworkedPlayerInfo.TaskInfo> tasks) { }

	[IteratorStateMachine(typeof(PlayerControl.<CoSetTasks>d__103))]
	// RVA: 0x5C1300 Offset: 0x5C0700 VA: 0x105C1300
	private IEnumerator CoSetTasks(List<NetworkedPlayerInfo.TaskInfo> tasks) { }

	// RVA: 0x5BE200 Offset: 0x5BD600 VA: 0x105BE200
	public PlayerTask AddSystemTask(SystemTypes system) { }

	// RVA: 0x5C6A80 Offset: 0x5C5E80 VA: 0x105C6A80
	public void RemoveTask(PlayerTask task) { }

	// RVA: 0x5C05C0 Offset: 0x5BF9C0 VA: 0x105C05C0
	public void ClearTasks() { }

	// RVA: 0x5C1A30 Offset: 0x5C0E30 VA: 0x105C1A30
	public void Die(DeathReason reason, bool assignGhostRole) { }

	// RVA: 0x5C77E0 Offset: 0x5C6BE0 VA: 0x105C77E0
	public void Revive() { }

	// RVA: 0x5C5920 Offset: 0x5C4D20 VA: 0x105C5920
	public void OnClick() { }

	// RVA: 0x5BF980 Offset: 0x5BED80 VA: 0x105BF980
	public void CheckSeekerHand_AnimEvent() { }

	// RVA: 0x5C5DE0 Offset: 0x5C51E0 VA: 0x105C5DE0
	public void PlayAnimation(byte animType) { }

	// RVA: 0x5C1360 Offset: 0x5C0760 VA: 0x105C1360
	public void CompleteTask(uint idx) { }

	// RVA: 0x5BE500 Offset: 0x5BD900 VA: 0x105BE500
	public bool AllTasksCompleted() { }

	// RVA: 0x5C5770 Offset: 0x5C4B70 VA: 0x105C5770
	public bool MustCleanVent(int ventId) { }

	[IteratorStateMachine(typeof(PlayerControl.<CoSetRole>d__116))]
	// RVA: 0x5C12A0 Offset: 0x5C06A0 VA: 0x105C12A0
	private IEnumerator CoSetRole(RoleTypes role, bool canOverride) { }

	// RVA: 0x5C2200 Offset: 0x5C1600 VA: 0x105C2200
	public void Exiled() { }

	// RVA: 0x5BF330 Offset: 0x5BE730 VA: 0x105BF330
	public void CheckName(string playerName) { }

	// RVA: 0x5C9F30 Offset: 0x5C9330 VA: 0x105C9F30
	public void SetKinematic(bool b) { }

	// RVA: 0x5CA000 Offset: 0x5C9400 VA: 0x105CA000
	public void SetName(string playerName) { }

	[IteratorStateMachine(typeof(PlayerControl.<CoSetName>d__121))]
	// RVA: 0x5C1240 Offset: 0x5C0640 VA: 0x105C1240
	private IEnumerator CoSetName(string playerName) { }

	// RVA: 0x5BEBF0 Offset: 0x5BDFF0 VA: 0x105BEBF0
	public void CheckColor(byte bodyColor) { }

	// RVA: 0x5C9AC0 Offset: 0x5C8EC0 VA: 0x105C9AC0
	public void SetHatAndVisorAlpha(float a) { }

	// RVA: 0x5C98E0 Offset: 0x5C8CE0 VA: 0x105C98E0
	public void SetColor(int bodyColor) { }

	[IteratorStateMachine(typeof(PlayerControl.<CoSetColor>d__125))]
	// RVA: 0x5C1110 Offset: 0x5C0510 VA: 0x105C1110
	private IEnumerator CoSetColor(int bodyColor) { }

	// RVA: 0x5C9FC0 Offset: 0x5C93C0 VA: 0x105C9FC0
	public void SetNamePlate(string namePlateId) { }

	// RVA: 0x5CAC50 Offset: 0x5CA050 VA: 0x105CAC50
	public void SetVisor(string visorId, int colorId) { }

	// RVA: 0x5C9F60 Offset: 0x5C9360 VA: 0x105C9F60
	public void SetLevel(uint level) { }

	[IteratorStateMachine(typeof(PlayerControl.<CoSetLevel>d__129))]
	// RVA: 0x5C11E0 Offset: 0x5C05E0 VA: 0x105C11E0
	private IEnumerator CoSetLevel(uint level) { }

	// RVA: 0x5CABA0 Offset: 0x5C9FA0 VA: 0x105CABA0
	public void SetSkin(string skinId, int color) { }

	// RVA: 0x5C9B30 Offset: 0x5C8F30 VA: 0x105C9B30
	public void SetHat(string hatId, int colorId) { }

	// RVA: 0x5CA290 Offset: 0x5C9690 VA: 0x105CA290
	public void SetPet(string petId) { }

	// RVA: 0x5CA260 Offset: 0x5C9660 VA: 0x105CA260
	public void SetPet(string petId, int colorId) { }

	// RVA: 0x5CA220 Offset: 0x5C9620 VA: 0x105CA220
	public void SetPetPosition(Vector3 newPosition) { }

	// RVA: 0x5C3870 Offset: 0x5C2C70 VA: 0x105C3870
	public PetBehaviour GetPet() { }

	// RVA: 0x5CAAE0 Offset: 0x5C9EE0 VA: 0x105CAAE0
	public static void SetSkinImageAsync(SkinData skin, int colorId, SpriteRenderer target) { }

	// RVA: 0x5C6D70 Offset: 0x5C6170 VA: 0x105C6D70
	private void ReportDeadBody(NetworkedPlayerInfo target) { }

	// RVA: 0x5C00D0 Offset: 0x5BF4D0 VA: 0x105C00D0
	private void CheckUseZipline(PlayerControl target, ZiplineBehaviour ziplineBehaviour, bool fromTop) { }

	// RVA: 0x5BFEB0 Offset: 0x5BF2B0 VA: 0x105BFEB0
	private void CheckSporeTrigger(Mushroom mushroom) { }

	// RVA: 0x5CB660 Offset: 0x5CAA60 VA: 0x105CB660
	public void StartMeeting(NetworkedPlayerInfo target) { }

	// RVA: 0x5C70E0 Offset: 0x5C64E0 VA: 0x105C70E0
	public void ResetForMeeting() { }

	// RVA: 0x5BEE10 Offset: 0x5BE210 VA: 0x105BEE10
	public void CheckMurder(PlayerControl target) { }

	// RVA: 0x5BF9F0 Offset: 0x5BEDF0 VA: 0x105BF9F0
	private void CheckShapeshift(PlayerControl target, bool shouldAnimate) { }

	// RVA: 0x5C47D0 Offset: 0x5C3BD0 VA: 0x105C47D0
	public bool IsMushroomMixupActive() { }

	// RVA: 0x5BE740 Offset: 0x5BDB40 VA: 0x105BE740
	public bool AreCommsAffected() { }

	// RVA: 0x5C4C80 Offset: 0x5C4080 VA: 0x105C4C80
	public void MurderPlayer(PlayerControl target, MurderResultFlags resultFlags) { }

	// RVA: 0x5BF5E0 Offset: 0x5BE9E0 VA: 0x105BF5E0
	public void CheckProtect(PlayerControl target) { }

	// RVA: 0x5C61F0 Offset: 0x5C55F0 VA: 0x105C61F0
	public void ProtectPlayer(PlayerControl target, int colorId) { }

	// RVA: 0x5CA360 Offset: 0x5C9760 VA: 0x105CA360
	public void SetPlayerMaterialColors(Renderer rend) { }

	// RVA: 0x5C4560 Offset: 0x5C3960 VA: 0x105C4560
	public static void HideCursorTemporarily() { }

	// RVA: 0x5C9430 Offset: 0x5C8830 VA: 0x105C9430
	public void SetAppearanceFromSaveData() { }

	// RVA: 0x5CB8E0 Offset: 0x5CACE0 VA: 0x105CB8E0
	public void ToggleHighlight(bool active, RoleTeamTypes targeterTeam) { }

	// RVA: 0x5CA060 Offset: 0x5C9460 VA: 0x105CA060
	public void SetOutfit(NetworkedPlayerInfo.PlayerOutfit newOutfit, PlayerOutfitType type) { }

	// RVA: 0x5C6520 Offset: 0x5C5920 VA: 0x105C6520
	public void RawSetOutfit(NetworkedPlayerInfo.PlayerOutfit newOutfit, PlayerOutfitType type) { }

	// RVA: 0x5C6910 Offset: 0x5C5D10 VA: 0x105C6910
	public void RejectShapeshift() { }

	// RVA: 0x5CACA0 Offset: 0x5CA0A0 VA: 0x105CACA0
	public void Shapeshift(PlayerControl targetPlayer, bool animate) { }

	// RVA: 0x3ADF60 Offset: 0x3AD360 VA: 0x103ADF60
	public int ShapeshiftedPlayerID() { }

	// RVA: 0x5CA400 Offset: 0x5C9800 VA: 0x105CA400
	private void SetRoleInvisibility(bool isActive, bool shouldAnimate = True, bool playFullAnimation = False) { }

	// RVA: 0x5C48A0 Offset: 0x5C3CA0 VA: 0x105C48A0
	private void LoopChargeAnim(PhantomRole phantomRole) { }

	// RVA: 0x5C35C0 Offset: 0x5C29C0 VA: 0x105C35C0
	public void ForcePhantomVisible() { }

	// RVA: 0x5BE610 Offset: 0x5BDA10 VA: 0x105BE610
	private void AppearOtherClients() { }

	// RVA: 0x5C1790 Offset: 0x5C0B90 VA: 0x105C1790
	private void ContinueVanish() { }

	// RVA: 0x5C9B60 Offset: 0x5C8F60 VA: 0x105C9B60
	private void SetInvisibility(bool isActive) { }

	// RVA: 0x5CB870 Offset: 0x5CAC70 VA: 0x105CB870
	public void StartPlayerTracking(PlayerControl playerToTrack, int colorID) { }

	// RVA: 0x5BEAA0 Offset: 0x5BDEA0 VA: 0x105BEAA0
	public void CancelPlayerTracking() { }

	// RVA: 0x5C1FF0 Offset: 0x5C13F0 VA: 0x105C1FF0
	public void DisableCurrentTrackers() { }

	// RVA: 0x5C4AA0 Offset: 0x5C3EA0 VA: 0x105C4AA0
	public void MixUpOutfit(NetworkedPlayerInfo.PlayerOutfit playerOutfit) { }

	[IteratorStateMachine(typeof(PlayerControl.<ScalePlayer>d__168))]
	// RVA: 0x5C9360 Offset: 0x5C8760 VA: 0x105C9360
	public IEnumerator ScalePlayer(float targetScale, float duration) { }

	// RVA: 0x5C0B80 Offset: 0x5BFF80 VA: 0x105C0B80
	public void CmdCheckRevertShapeshift(bool shouldAnimate) { }

	// RVA: 0x5C2280 Offset: 0x5C1680 VA: 0x105C2280
	public void FixMixedUpOutfit() { }

	// RVA: 0x5C64C0 Offset: 0x5C58C0 VA: 0x105C64C0
	private void RawSetName(string name) { }

	// RVA: 0x5C6400 Offset: 0x5C5800 VA: 0x105C6400
	private void RawSetColor(int bodyColor) { }

	// RVA: 0x5C6720 Offset: 0x5C5B20 VA: 0x105C6720
	private void RawSetVisor(string visorId, int color) { }

	// RVA: 0x5C66F0 Offset: 0x5C5AF0 VA: 0x105C66F0
	private void RawSetSkin(string skinId, int color) { }

	// RVA: 0x5C6430 Offset: 0x5C5830 VA: 0x105C6430
	private void RawSetHat(string hatId, int colorId) { }

	// RVA: 0x5C6670 Offset: 0x5C5A70 VA: 0x105C6670
	private void RawSetPet(string petId, int colorId) { }

	// RVA: 0x5CB560 Offset: 0x5CA960 VA: 0x105CB560
	private void ShowFailedMurder() { }

	// RVA: 0x5C6920 Offset: 0x5C5D20 VA: 0x105C6920
	private void RemoveProtection() { }

	// RVA: 0x5CBC00 Offset: 0x5CB000 VA: 0x105CBC00
	private void TurnOnProtection(bool visible, int colorId, int guardianPlayerId) { }

	// RVA: 0x5C97F0 Offset: 0x5C8BF0 VA: 0x105C97F0
	private void SetColorBlindTag() { }

	// RVA: 0x5BEA10 Offset: 0x5BDE10 VA: 0x105BEA10
	public bool CanPet() { }

	// RVA: 0x5BE330 Offset: 0x5BD730 VA: 0x105BE330
	public void AdjustLighting() { }

	// RVA: 0x5C4600 Offset: 0x5C3A00 VA: 0x105C4600
	private bool IsFlashlightEnabled() { }

	// RVA: 0x5C9940 Offset: 0x5C8D40 VA: 0x105C9940
	private void SetFlashlightInputMethod() { }

	[IteratorStateMachine(typeof(PlayerControl.<EnableRightJoystick>d__187))]
	// RVA: 0x5C21B0 Offset: 0x5C15B0 VA: 0x105C21B0
	private IEnumerator EnableRightJoystick(bool enabled) { }

	[IteratorStateMachine(typeof(PlayerControl.<CoSetCosmetic>d__188))]
	// RVA: 0x5C1170 Offset: 0x5C0570 VA: 0x105C1170
	private IEnumerator CoSetCosmetic(RpcCalls rpc, string cosmeticId, byte sequenceId) { }

	// RVA: 0x5CB380 Offset: 0x5CA780 VA: 0x105CB380
	private bool ShouldProcessRpc(RpcCalls rpc, byte sequenceId) { }

	// RVA: 0x5C35E0 Offset: 0x5C29E0 VA: 0x105C35E0
	private byte GetNextRpcSequenceId(RpcCalls rpc) { }

	// RVA: 0x5C89C0 Offset: 0x5C7DC0 VA: 0x105C89C0
	public void RpcSetScanner(bool value) { }

	// RVA: 0x5C90F0 Offset: 0x5C84F0 VA: 0x105C90F0
	public void RpcUsePlatform() { }

	// RVA: 0x5C0DD0 Offset: 0x5C01D0 VA: 0x105C0DD0
	public void CmdCheckUseZipline(PlayerControl target, ZiplineBehaviour ziplineBehaviour, bool fromTop) { }

	// RVA: 0x5C0D10 Offset: 0x5C0110 VA: 0x105C0D10
	public void CmdCheckSporeTrigger(Mushroom mushroom) { }

	// RVA: 0x5C7C90 Offset: 0x5C7090 VA: 0x105C7C90
	public void RpcPlayAnimation(byte animType) { }

	// RVA: 0x5C8C10 Offset: 0x5C8010 VA: 0x105C8C10
	public void RpcSetStartCounter(int secondsLeft) { }

	// RVA: 0x5C7B30 Offset: 0x5C6F30 VA: 0x105C7B30
	public void RpcCompleteTask(uint idx) { }

	// RVA: 0x5C88D0 Offset: 0x5C7CD0 VA: 0x105C88D0
	public void RpcSetRole(RoleTypes roleType, bool canOverrideRole = False) { }

	// RVA: 0x5C0A20 Offset: 0x5BFE20 VA: 0x105C0A20
	public void CmdCheckName(string name) { }

	// RVA: 0x5C8530 Offset: 0x5C7930 VA: 0x105C8530
	public void RpcSetLevel(uint level) { }

	// RVA: 0x5C8CA0 Offset: 0x5C80A0 VA: 0x105C8CA0
	public void RpcSetVisor(string visorId) { }

	// RVA: 0x5C85D0 Offset: 0x5C79D0 VA: 0x105C85D0
	public void RpcSetNamePlate(string namePlateId) { }

	// RVA: 0x5C8AD0 Offset: 0x5C7ED0 VA: 0x105C8AD0
	public void RpcSetSkin(string skinId) { }

	// RVA: 0x5C8400 Offset: 0x5C7800 VA: 0x105C8400
	public void RpcSetHat(string hatId) { }

	// RVA: 0x5C8760 Offset: 0x5C7B60 VA: 0x105C8760
	public void RpcSetPet(string petId) { }

	// RVA: 0x5C86A0 Offset: 0x5C7AA0 VA: 0x105C86A0
	public void RpcSetName(string name) { }

	// RVA: 0x5C08A0 Offset: 0x5BFCA0 VA: 0x105C08A0
	public void CmdCheckColor(byte bodyColor) { }

	// RVA: 0x5C8340 Offset: 0x5C7740 VA: 0x105C8340
	public void RpcSetColor(byte bodyColor) { }

	// RVA: 0x5C7FD0 Offset: 0x5C73D0 VA: 0x105C7FD0
	public bool RpcSendChat(string chatText) { }

	// RVA: 0x5C81A0 Offset: 0x5C75A0 VA: 0x105C81A0
	public bool RpcSendQuickChat(QuickChatPhraseBuilderResult data) { }

	// RVA: 0x5C7ED0 Offset: 0x5C72D0 VA: 0x105C7ED0
	public void RpcSendChatNote(byte srcPlayerId, ChatNoteTypes noteType) { }

	// RVA: 0x5C1050 Offset: 0x5C0450 VA: 0x105C1050
	public void CmdReportDeadBody(NetworkedPlayerInfo target) { }

	// RVA: 0x5C8EA0 Offset: 0x5C82A0 VA: 0x105C8EA0
	public void RpcStartMeeting(NetworkedPlayerInfo info) { }

	// RVA: 0x5C0950 Offset: 0x5BFD50 VA: 0x105C0950
	public void CmdCheckMurder(PlayerControl target) { }

	// RVA: 0x5C0AD0 Offset: 0x5BFED0 VA: 0x105C0AD0
	public void CmdCheckProtect(PlayerControl target) { }

	// RVA: 0x5C7BD0 Offset: 0x5C6FD0 VA: 0x105C7BD0
	public void RpcMurderPlayer(PlayerControl target, bool didSucceed) { }

	// RVA: 0x5C9230 Offset: 0x5C8630 VA: 0x105C9230
	public void RpcUseZipline(PlayerControl target, ZiplineBehaviour ziplineBehaviour, bool fromTop) { }

	// RVA: 0x5C9030 Offset: 0x5C8430 VA: 0x105C9030
	public void RpcTriggerSpores(Mushroom mushroom) { }

	// RVA: 0x5C7D80 Offset: 0x5C7180 VA: 0x105C7D80
	public void RpcProtectPlayer(PlayerControl target, int colorId) { }

	// RVA: 0x5C8DE0 Offset: 0x5C81E0 VA: 0x105C8DE0
	public void RpcShapeshift(PlayerControl target, bool shouldAnimate) { }

	// RVA: 0x5C0C50 Offset: 0x5C0050 VA: 0x105C0C50
	public void CmdCheckShapeshift(PlayerControl target, bool shouldAnimate) { }

	// RVA: 0x5C7E40 Offset: 0x5C7240 VA: 0x105C7E40
	public void RpcRejectShapeshift() { }

	// RVA: 0x5C8F70 Offset: 0x5C8370 VA: 0x105C8F70
	public void RpcSyncSettings(byte[] optionsByteArray) { }

	// RVA: 0x335100 Offset: 0x334500 VA: 0x10335100 Slot: 8
	public override void ClearOrDecrementDirt() { }

	// RVA: 0x5C93D0 Offset: 0x5C87D0 VA: 0x105C93D0 Slot: 9
	public override bool Serialize(MessageWriter writer, bool initialState) { }

	// RVA: 0x5C19E0 Offset: 0x5C0DE0 VA: 0x105C19E0 Slot: 10
	public override void Deserialize(MessageReader reader, bool initialState) { }

	// RVA: 0x5C3950 Offset: 0x5C2D50 VA: 0x105C3950 Slot: 7
	public override void HandleRpc(byte callId, MessageReader reader) { }

	// RVA: 0x5C3910 Offset: 0x5C2D10 VA: 0x105C3910 Slot: 11
	public virtual void HandleRoleRpc(byte callId, MessageReader reader) { }

	// RVA: 0x5C4420 Offset: 0x5C3820 VA: 0x105C4420
	public void HandleServerVanish() { }

	// RVA: 0x5C4400 Offset: 0x5C3800 VA: 0x105C4400
	public void HandleServerAppear(bool shouldAnimate) { }

	// RVA: 0x5C04E0 Offset: 0x5BF8E0 VA: 0x105C04E0
	public void CheckVanish() { }

	// RVA: 0x5C92F0 Offset: 0x5C86F0 VA: 0x105C92F0
	public void RpcVanish() { }

	// RVA: 0x5BEAF0 Offset: 0x5BDEF0 VA: 0x105BEAF0
	public void CheckAppear(bool shouldAnimate) { }

	// RVA: 0x5C7AB0 Offset: 0x5C6EB0 VA: 0x105C7AB0
	public void RpcAppear(bool shouldAnimate) { }

	// RVA: 0x5C0EB0 Offset: 0x5C02B0 VA: 0x105C0EB0
	public void CmdCheckVanish(float maxDuration) { }

	// RVA: 0x5C0720 Offset: 0x5BFB20 VA: 0x105C0720
	public void CmdCheckAppear(bool shouldAnimate) { }

	// RVA: 0x5CC2D0 Offset: 0x5CB6D0 VA: 0x105CC2D0
	public void .ctor() { }

	// RVA: 0x5CC270 Offset: 0x5CB670 VA: 0x105CC270
	private static void .cctor() { }

	[CompilerGenerated]
	// RVA: 0x5CC050 Offset: 0x5CB450 VA: 0x105CC050
	private bool <Start>b__82_0() { }

	[CompilerGenerated]
	// RVA: 0x5CC060 Offset: 0x5CB460 VA: 0x105CC060
	private bool <Start>b__82_2() { }

	[CompilerGenerated]
	// RVA: 0x5CBF10 Offset: 0x5CB310 VA: 0x105CBF10
	private bool <ClientInitialize>b__83_0() { }

	[CompilerGenerated]
	// RVA: 0x5CBFD0 Offset: 0x5CB3D0 VA: 0x105CBFD0
	private void <CoSetName>b__121_0() { }

	[CompilerGenerated]
	// RVA: 0x5CC010 Offset: 0x5CB410 VA: 0x105CC010
	private void <RawSetPet>b__176_0() { }
}


// Namespace: 
public class NetworkedPlayerInfo : InnerNetObject // TypeDefIndex: 693
{
	// Fields
	private const int MAX_NETOBJECT_INIT_ATTEMPTS = 60;
	private const float NETOBJECT_INIT_ATTEMPT_WAIT_SECONDS = 0.1;
	public byte PlayerId; // 0x28
	public int ClientId; // 0x2C
	public string FriendCode; // 0x30
	public string Puid; // 0x34
	public RoleTypes RoleType; // 0x38
	public Nullable<RoleTypes> RoleWhenAlive; // 0x3A
	public Dictionary<PlayerOutfitType, NetworkedPlayerInfo.PlayerOutfit> Outfits; // 0x40
	public uint PlayerLevel; // 0x44
	public bool Disconnected; // 0x48
	public RoleBehaviour Role; // 0x4C
	public List<NetworkedPlayerInfo.TaskInfo> Tasks; // 0x50
	public bool IsDead; // 0x54
	public bool WasEjected; // 0x55
	private PlayerControl _object; // 0x58

	// Properties
	public bool IsIncomplete { get; }
	public PlayerControl Object { get; }
	public NetworkedPlayerInfo.PlayerOutfit DefaultOutfit { get; }
	public string PlayerName { get; set; }
	public Color Color { get; }
	public Color ShadowColor { get; }
	public string ColorName { get; }

	// Methods

	// RVA: 0x7D5F60 Offset: 0x7D5360 VA: 0x107D5F60
	public bool get_IsIncomplete() { }

	// RVA: 0x7D6090 Offset: 0x7D5490 VA: 0x107D6090
	public PlayerControl get_Object() { }

	// RVA: 0x7D5F10 Offset: 0x7D5310 VA: 0x107D5F10
	public NetworkedPlayerInfo.PlayerOutfit get_DefaultOutfit() { }

	// RVA: 0x7D6160 Offset: 0x7D5560 VA: 0x107D6160
	public string get_PlayerName() { }

	// RVA: 0x7D6280 Offset: 0x7D5680 VA: 0x107D6280
	public void set_PlayerName(string value) { }

	// RVA: 0x7D5E10 Offset: 0x7D5210 VA: 0x107D5E10
	public Color get_Color() { }

	// RVA: 0x7D6180 Offset: 0x7D5580 VA: 0x107D6180
	public Color get_ShadowColor() { }

	// RVA: 0x7D5DC0 Offset: 0x7D51C0 VA: 0x107D5DC0
	public string get_ColorName() { }

	// RVA: 0x7D4F00 Offset: 0x7D4300 VA: 0x107D4F00
	public void Init(PlayerControl pc, int clientId) { }

	// RVA: 0x7D4F50 Offset: 0x7D4350 VA: 0x107D4F50
	public void MarkDirty() { }

	// RVA: 0x507DC0 Offset: 0x5071C0 VA: 0x10507DC0 Slot: 8
	public override void ClearOrDecrementDirt() { }

	// RVA: 0x7D5120 Offset: 0x7D4520 VA: 0x107D5120 Slot: 9
	public override bool Serialize(MessageWriter writer, bool initialState) { }

	// RVA: 0x7D4730 Offset: 0x7D3B30 VA: 0x107D4730 Slot: 10
	public override void Deserialize(MessageReader reader, bool initialState) { }

	// RVA: 0x7D4E40 Offset: 0x7D4240 VA: 0x107D4E40
	public string GetPlayerName(PlayerOutfitType outfitType) { }

	// RVA: 0x7D4610 Offset: 0x7D3A10 VA: 0x107D4610
	public void CensorNameAsync(Action callback) { }

	[IteratorStateMachine(typeof(NetworkedPlayerInfo.<CoCensorNameAsync>d__38))]
	// RVA: 0x7D4670 Offset: 0x7D3A70 VA: 0x107D4670
	private IEnumerator CoCensorNameAsync(Action callback) { }

	// RVA: 0x7D4CB0 Offset: 0x7D40B0 VA: 0x107D4CB0
	public string GetPlayerColorString(PlayerOutfitType outfitType = 0) { }

	// RVA: 0x7D4C10 Offset: 0x7D4010 VA: 0x107D4C10
	public NetworkedPlayerInfo.TaskInfo FindTaskById(uint taskId) { }

	// RVA: 0x7D5550 Offset: 0x7D4950 VA: 0x107D5550
	public void SetOutfit(PlayerOutfitType outfitType, NetworkedPlayerInfo.PlayerOutfit outfit) { }

	// RVA: 0x7D5A70 Offset: 0x7D4E70 VA: 0x107D5A70
	public void UpdateNamePlate(string namePlate) { }

	// RVA: 0x7D5C70 Offset: 0x7D5070 VA: 0x107D5C70
	public void UpdateVisor(string visor) { }

	// RVA: 0x7D5C00 Offset: 0x7D5000 VA: 0x107D5C00
	public void UpdateSkin(string skin) { }

	// RVA: 0x7D5B90 Offset: 0x7D4F90 VA: 0x107D5B90
	public void UpdatePet(string petId) { }

	// RVA: 0x7D5890 Offset: 0x7D4C90 VA: 0x107D5890
	public void UpdateHat(string hat) { }

	// RVA: 0x7D5830 Offset: 0x7D4C30 VA: 0x107D5830
	public void UpdateColor(int colorId) { }

	[IteratorStateMachine(typeof(NetworkedPlayerInfo.<CoUpdateColor>d__48))]
	// RVA: 0x7D46D0 Offset: 0x7D3AD0 VA: 0x107D46D0
	private IEnumerator CoUpdateColor(int colorId) { }

	// RVA: 0x7D5900 Offset: 0x7D4D00 VA: 0x107D5900
	private void UpdateHostPanelImage() { }

	// RVA: 0x7D5A40 Offset: 0x7D4E40 VA: 0x107D5A40
	public void UpdateLevel(uint level) { }

	// RVA: 0x7D5AD0 Offset: 0x7D4ED0 VA: 0x107D5AD0
	private void UpdateName(string playerName, ClientData client) { }

	// RVA: 0x7D5000 Offset: 0x7D4400 VA: 0x107D5000
	public void OnGameStart() { }

	// RVA: 0x7D4F70 Offset: 0x7D4370 VA: 0x107D4F70
	public void OnGameEnd() { }

	// RVA: 0x7D55A0 Offset: 0x7D49A0 VA: 0x107D55A0
	private void SetTasks(byte[] taskTypeIds) { }

	// RVA: 0x7D5080 Offset: 0x7D4480 VA: 0x107D5080
	public void RpcSetTasks(byte[] taskTypeIds) { }

	// RVA: 0x7D4ED0 Offset: 0x7D42D0 VA: 0x107D4ED0 Slot: 7
	public override void HandleRpc(byte callId, MessageReader reader) { }

	// RVA: 0x7D5CE0 Offset: 0x7D50E0 VA: 0x107D5CE0
	public void .ctor() { }

	[CompilerGenerated]
	// RVA: 0x515860 Offset: 0x514C60 VA: 0x10515860
	private bool <get_Object>b__19_0(PlayerControl p) { }
}
