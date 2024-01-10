import { useEffect, useId, useState } from 'react'
import './reset.css'
import './App.css'
import Home from './pages/Home'
import Lobby from './pages/Lobby'
import Game from './pages/Game/Game'
import { ws, initWebSocket, Client, Server, joinLobby, startGame } from './clientMessage'

const Page = {
  Home: 0,
  Lobby: 1,
  Game: 2
}

function App() {
  const [page, setPage] = useState(Page.Home)
  const [userId, setUserId] = useState(-1)
  const [roomId, setRoomId] = useState()
  const [lobby, setLobby] = useState()
  const [game, setGame] = useState()

  function onMessage(e) {
    console.log(e.data)
    const message = JSON.parse(e.data)
    const payload = message['payload'];

    switch(message['messageType']) {
      case Server.MessageType.UserId:
        setUserId(payload['userId'])
        break;
      case Server.MessageType.Lobby:
        setPage(Page.Lobby)
        setLobby(payload)
        break;
      case Server.MessageType.Game:
        console.log(JSON.stringify(payload))
        setPage(Page.Game)
        setGame(payload)
        if (payload.isDone) {
          ws.close()
        }
        break;
    }

  }

  useEffect(() => {
    const url = window.location.search
    const params = new URLSearchParams(url)
    const roomId = params.get('roomId')
    console.log(roomId, roomId !== null)
    setRoomId(roomId)

    let onOpen = null;
    if (roomId) {
      onOpen = () => joinLobby(roomId)
    }

    initWebSocket(onMessage, onOpen)

    return () => {
      if (ws) {
        ws.close()
      }
    }
  }, [])

  function onPlay(roomId) {
    joinLobby(roomId)
  }

  function onStart() {
    startGame()
  }

  // const mockGame = {"activeMerchantCards":[{"crystals":[0,0,0,0],"id":2},{"crystals":[0,0,0,0],"id":3},{"crystals":[0,0,0,0],"id":4},{"crystals":[0,0,0,0],"id":5},{"crystals":[0,0,0,0],"id":6},{"crystals":[0,0,0,0],"id":7}],"activePointCardIds":[0,1,2,3,4],"id":"0ZZ2prGE","isDone":false,"lastRound":false,"maxGolems":6,"numCopperTokens":4,"numSilverTokens":4,"players":[{"crystals":[4, 0, 0, 0],"id":19,"merchantCardIds":[0,1],"numCopperTokens":0,"numSilverTokens":0,"pointCardIds":[],"turn":1,"usedMerchantCardIds":[]},{"crystals":[4, 0, 0, 0],"id":21,"merchantCardIds":[0,1],"numCopperTokens":0,"numSilverTokens":0,"pointCardIds":[],"turn":0,"usedMerchantCardIds":[]}],"turn":0}

  return (
    <>
      {/* {<Game userId={mockGame.players[1].id} {...mockGame} />} */}
      {page === Page.Home && (
        <Home onPlay={onPlay} />
      )}
      {page === Page.Lobby && lobby && (
        <Lobby userId={userId} {...lobby} onStart={onStart} />
      )}
      {page === Page.Game && game && (
        <Game userId={userId} {...game} />
      )}
    </>
  )
}

export default App
