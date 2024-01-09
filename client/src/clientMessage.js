// FIXME: change name

const Client = {
    MessageType: {
        JoinLobby: 0,
        StartGame: 1,
        Move: 2
    }
}

const Game = {
    MoveType: {
        PlayMove: 0,
        AcquireMove: 1,
        RestMove: 2,
        ClaimMove: 3
    },
    PlayMoveType: {
        CrystalPlayMove: 0,
        UpgradePlayMove: 1,
        TradePlayMove: 2
    }
}

const ws = new WebSocket('ws://localhost:9001')

function padNum(id) {
    if (id < 10) {
        return `0${id}`
    }

    return `${id}`
}

function randomId(length = 8) {
    const characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789'
    let randomId = ''
    
    for (let i = 0; i < length; i++) {
      const randomIndex = Math.floor(Math.random() * characters.length)
      randomId += characters.charAt(randomIndex)
    }
  
    return randomId
}

function joinLobby(roomId) {
    const messageParts = [
        Client.MessageType.JoinLobby,
        roomId
    ]

    ws.send(messageParts.join(''))
}

function startGame() {
    const messageParts = [
        Client.MessageType.StartGame
    ]

    ws.send(messageParts.join(''))
}

function acquireMove(merchantCardId, crystals) {
    const messageParts = [
        Client.MessageType.Move,
        Game.MoveType.AcquireMove,
        padNum(merchantCardId),
        crystals.length,
        crystals.join('')
    ]

    ws.send(messageParts.join(''))
}

function restMove() {
    const messageParts = [
        Client.MessageType.Move,
        Game.MoveType.RestMove
    ]

    ws.send(messageParts.join(''))
}

function claimMove(pointCardId) {
    const messageParts = [
        Client.MessageType.Move,
        Game.MoveType.ClaimMove,
        padNum(pointCardId)
    ]

    ws.send(messageParts.join(''))
}

function crystalPlayMove(merchantCardId) {
    const messageParts = [
        Client.MessageType.Move,
        Game.MoveType.PlayMove,
        Game.PlayMoveType.CrystalPlayMove,
        padNum(merchantCardId)
    ]

    ws.send(messageParts.join(''))
}

function upgradePlayMove(merchantCardId, upgrades) {
    const messageParts = [
        Client.MessageType.Move,
        Game.MoveType.PlayMove,
        Game.PlayMoveType.UpgradePlayMove,
        padNum(merchantCardId),
        upgrades.length,
        upgrades.join('')
    ]

    ws.send(messageParts.join(''))
}

function tradePlayMove(merchantCardId, numTrades) {
    const messageParts = [
        Client.MessageType.Move,
        Game.MoveType.PlayMove,
        Game.PlayMoveType.TradePlayMove,
        padNum(merchantCardId),
        padNum(numTrades)
    ]

    ws.send(messageParts.join(''))
}

export {
    ws,
    randomId,
    joinLobby,
    startGame,
    acquireMove,
    restMove,
    claimMove,
    crystalPlayMove,
    upgradePlayMove,
    tradePlayMove
}