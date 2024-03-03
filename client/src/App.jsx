import { useEffect, useId, useState } from 'react'
import './reset.css'
import './App.css'
import Home from './pages/Home/Home'
import Lobby from './pages/Lobby/Lobby'
import Game from './pages/Game/Game'
import { ws, initWebSocket, Client, Server, joinLobby, startGame } from './clientMessage'
import mockGame from './models/MockGame.json'

const Page = {
  Home: 0,
  Lobby: 1,
  Game: 2
}

function App() {
  const [page, setPage] = useState(Page.Home)
  const [userId, setUserId] = useState(-1)
  const [userName, setUserName] = useState('')
  const [lobby, setLobby] = useState()
  const [game, setGame] = useState()

  function onMessage(e) {
    const message = JSON.parse(e.data)
    const payload = message['payload'];

    switch(message['messageType']) {
      case Server.MessageType.UserId:
        setUserId(payload['userId'])
        setUserName(payload['userName'])
        break;
      case Server.MessageType.Lobby:
        setPage(Page.Lobby)
        setLobby(payload)
        break;
      case Server.MessageType.Game:
        setPage(Page.Game)
        setGame(payload)
        break;
    }

  }

  useEffect(() => {
    const url = window.location.search
    const params = new URLSearchParams(url)
    const roomId = params.get('roomId')

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

  function onHome() {
    setPage(Page.Home)
  }
  
  return (
    <>
      {page === Page.Home && userName && (
        <Home userName={userName} onPlay={onPlay} />
      )}
      {page === Page.Lobby && lobby && (
        <Lobby userId={userId} {...lobby} onStart={onStart} />
      )}
      {page === Page.Game && game && (
        <Game
          userId={userId}
          {...game}
          onHome={onHome}
        />
      )}
    </>
  )
}

export default App
