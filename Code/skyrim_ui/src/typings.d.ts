declare const module: NodeModule;

interface NodeModule {
  id: string;
}

/** Skyim: Together type definitions */
declare namespace SkyrimTogetherTypes {
  /** Client initialization callback */
  type InitCallback = () => void;

  /** UI activation callback */
  type ActivateCallback = () => void;

  /** UI deactivation callback */
  type DeactivateCallback = () => void;

  /** Player game entry callback */
  type EnterGameCallback = () => void;

  /** Player game exit callback */
  type ExitGameCallback = () => void;

  /** Player open/close game menu callback */
  type OpeningMenuCallback = (openingMenu: boolean) => void;

  /** Player message reception callback */
  type MessageCallback = (name: string, message: string) => void;

  /** System message reception callback */
  type SystemMessageCallback = (message: string) => void;

  /** Whisper message reception callback */
  type WhisperMessageCallback = (name: string, message: string) => void;

  /** Connection callback */
  type ConnectCallback = () => void;

  /** Disconnection callback */
  type DisconnectCallback = (isError: boolean) => void;

  /** Name change callback */
  type SetNameCallback = (name: string) => void;

  /** Version set callback */
  type SetVersionCallback = (version: string) => void;

  /**  */
  type OnDebugCallback = (isDebug: boolean) => void;

  type UpdateDebugCallback = (
    numPacketsSent: number,
    numPacketsReceived: number,
    RTT: number,
    packetLoss: number,
    sentBandwidth: number,
    receivedBandwidth: number
  ) => void;

  type UserDataSetCallback = (token: string, username: string) => void;

  type PlayerConnectedCallback = (serverId: number, username: string, level: number, cellName: string) => void;

  type PlayerDisconnectedCallback = (serverId: number, username: string) => void;

  type SetHealthCallback = (serverId: number, health: number) => void;

  type SetLevelCallback = (serverId: number, level: number) => void;

  type SetCellCallback = (serverId: number, cellName: string) => void;

  type SetPlayer3dLoadedCallback = (serverId: number, health: number) => void;

  type SetPlayer3dUnloadedCallback = (serverId: number) => void;

  type SetServerIdCallback = (serverId: number) => void;

  type ProtocolMismatch = () => void;

  type TriggerError = () => void;
}

/** Global Skyrim: Together object. */
declare const skyrimtogether: SkyrimTogether;

/** Global Skyrim: Together object type. */
interface SkyrimTogether {
  /** Add listener to when the UI is first initialized. */
  on(event: 'init', callback: SkyrimTogetherTypes.InitCallback): void;

  /** Add listener to when the UI is activated. */
  on(event: 'activate', callback: SkyrimTogetherTypes.ActivateCallback): void;

  /** Add listener to when the UI is deactivated. */
  on(event: 'deactivate', callback: SkyrimTogetherTypes.DeactivateCallback): void;

  /** Add listener to when the player enters a game. */
  on(event: 'enterGame', callback: SkyrimTogetherTypes.EnterGameCallback): void;

  /** Add listener to when the player exits a game. */
  on(event: 'exitGame', callback: SkyrimTogetherTypes.ExitGameCallback): void;

  /** Add listener to when the player open/close a game menu. */
  on(event: 'openingMenu', callback: SkyrimTogetherTypes.OpeningMenuCallback): void;

  /** Add listener to when a player message is received. */
  on(event: 'message', callback: SkyrimTogetherTypes.MessageCallback): void;

  /** Add listener to when a system message is received. */
  on(event: 'systemMessage', callback: SkyrimTogetherTypes.SystemMessageCallback): void;

  /** Add listener to when a whisper message is received. */
  on(event: 'whisperMessage', callback: SkyrimTogetherTypes.WhisperMessageCallback): void;

  /** Add listener to when the player connects to a server. */
  on(event: 'connect', callback: SkyrimTogetherTypes.ConnectCallback): void;

  /** Add listener to when the player disconnects from a server. */
  on(event: 'disconnect', callback: SkyrimTogetherTypes.DisconnectCallback): void;

  /** Add listener to when the player's name changes. */
  on(event: 'setName', callback: SkyrimTogetherTypes.SetNameCallback): void;

  /** Add listener to when the client's version is set. */
  on(event: 'setVersion', callback: SkyrimTogetherTypes.SetVersionCallback): void;

  /** Add listener to when the player's press the F3 key */
  on(event: 'debug', callback: SkyrimTogetherTypes.OnDebugCallback): void;

  on(event: 'debugData', callback: SkyrimTogetherTypes.UpdateDebugCallback): void;

  /** Add listener to when the player's is connected with the launcher */
  on(event: 'userDataSet', callback: SkyrimTogetherTypes.UserDataSetCallback): void;

  /** Add listener to when one player connect in server. */
  on(event: 'playerConnected', callback: SkyrimTogetherTypes.PlayerConnectedCallback): void;

  /** Add listener to when one player disconnect in server. */
  on(event: 'playerDisconnected', callback: SkyrimTogetherTypes.PlayerDisconnectedCallback): void;

  on(event: 'setHealth', callback: SkyrimTogetherTypes.SetHealthCallback): void;

  /** Add listener to when one player change level in server. */
  on(event: 'setLevel', callback: SkyrimTogetherTypes.SetLevelCallback): void;

  /** Add listener to when one player change cell in server. */
  on(event: 'setCell', callback: SkyrimTogetherTypes.SetCellCallback): void;

  /** Add listener to when a player is loaded or unloaded in 3D.  */
  on(event: 'setPlayer3dLoaded', callback: SkyrimTogetherTypes.SetPlayer3dLoadedCallback): void;

  on(event: 'setPlayer3dUnloaded', callback: SkyrimTogetherTypes.SetPlayer3dUnloadedCallback): void;

  on(event: 'setServerId', callback: SkyrimTogetherTypes.SetServerIdCallback): void;

  on(event: 'protocolMismatch', callback: SkyrimTogetherTypes.ProtocolMismatch): void;

  on(event: 'triggerError', callback: SkyrimTogetherTypes.TriggerError): void;

  /** Remove listener from when the application is first initialized. */
  off(event: 'init', callback?: SkyrimTogetherTypes.InitCallback): void;

  /** Remove listener from when the UI is activated. */
  off(event: 'activate', callback?: SkyrimTogetherTypes.ActivateCallback): void;

  /** Remove listener from when the UI is deactivated. */
  off(event: 'deactivate', callback?: SkyrimTogetherTypes.DeactivateCallback): void;

  /** Remove listener from when the player enters a game. */
  off(event: 'enterGame', callback?: SkyrimTogetherTypes.EnterGameCallback): void;

  /** Remove listener from when the player exits a game. */
  off(event: 'exitGame', callback?: SkyrimTogetherTypes.ExitGameCallback): void;

  /** Add listener to when the player open/close a game menu. */
  off(event: 'openingMenu', callback?: SkyrimTogetherTypes.OpeningMenuCallback): void;

  /** Remove listener from when a player message is received. */
  off(event: 'message', callback?: SkyrimTogetherTypes.MessageCallback): void;

  /** Remove listener from when a system message is received. */
  off(event: 'systemMessage', callback?: SkyrimTogetherTypes.SystemMessageCallback): void;

  /** Remove listener from when a whisper message is received. */
  off(event: 'whisperMessage', callback?: SkyrimTogetherTypes.WhisperMessageCallback): void;

  /** Remove listener from when the player connects to a server. */
  off(event: 'connect', callback?: SkyrimTogetherTypes.ConnectCallback): void;

  /** Remove listener from when the player disconnects from a server. */
  off(event: 'disconnect', callback?: SkyrimTogetherTypes.DisconnectCallback): void;

  /** Remove listener from when the player's name changes. */
  off(event: 'setName', callback?: SkyrimTogetherTypes.SetNameCallback): void;

  /** Remove listener from when the client's version is set. */
  off(event: 'setVersion', callback?: SkyrimTogetherTypes.SetVersionCallback): void;

  /** Remove listener to when the player's press the F3 key */
  off(event: 'debug', callback?: SkyrimTogetherTypes.OnDebugCallback): void;

  off(event: 'debugData', callback?: SkyrimTogetherTypes.UpdateDebugCallback): void;

  /** Add listener to when one player connect in server. */
  off(event: 'playerConnected', callback?: SkyrimTogetherTypes.PlayerConnectedCallback): void;

  /** Add listener to when one player disconnect in server. */
  off(event: 'playerDisconnected', callback?: SkyrimTogetherTypes.PlayerDisconnectedCallback): void;

  off(event: 'userDataSet', callback?: SkyrimTogetherTypes.UserDataSetCallback): void;

  off(event: 'setHealth', callback?: SkyrimTogetherTypes.SetHealthCallback): void;

  off(event: 'setLevel', callback?: SkyrimTogetherTypes.SetLevelCallback): void;

  off(event: 'setCell', callback?: SkyrimTogetherTypes.SetCellCallback): void;

  /** Add listener to when a player is loaded or unloaded in 3D.  */
  off(event: 'setPlayer3dLoaded', callback?: SkyrimTogetherTypes.SetPlayer3dLoadedCallback): void;

  off(event: 'setPlayer3dUnloaded', callback?: SkyrimTogetherTypes.SetPlayer3dUnloadedCallback): void;

  off(event: 'setServerId', callback?: SkyrimTogetherTypes.SetServerIdCallback): void;

  off(event: 'protocolMismatch', callback?: SkyrimTogetherTypes.ProtocolMismatch): void;

  off(event: 'triggerError', callback?: SkyrimTogetherTypes.TriggerError): void;

  /**
   * Connect to server at given address and port.
   *
   * @param host IP address or hostname.
   * @param port Port.
   */
  connect(host: string, port: number, token: string): void;

  /**
   * Disconnect from server or cancel connection.
   */
  disconnect(): void;

  /**
   * Send message to server.
   *
   * @param message Message to send.
   */
  sendMessage(message: string): void;

  /**
   * Deactivate UI and release control.
   */
  deactivate(): void;

  /**
   * Teleport to given player
   * 
   * @param playerId Id of the player to which the requester should be teleported to
   */
  teleportToPlayer(playerId: number): void;

   /** 
   * Reconnect the client.
   */
  reconnect(): void;
}
