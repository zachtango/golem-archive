import { useEffect, useId, useState } from 'react'
import './reset.css'
import './App.css'
import Home from './pages/Home/Home'
import Lobby from './pages/Lobby/Lobby'
import Game from './pages/Game/Game'
import { ws, initWebSocket, Client, Server, joinLobby, startGame } from './clientMessage'
import mockGame from './models/MockGame.json'
import { AssetsContext } from './AssetsContext'

const Page = {
  Home: 0,
  Lobby: 1,
  Game: 2
}

function AssetsProvider({ children }) {
    const [assets, setAssets] = useState(null)

    useEffect(() => {
        const merchantCards = []
        const pointCards = []

        const loadAssets = async () => {
            for (let i = 0; i < 44; i++) {
                merchantCards.push(import(`./assets/merchant-cards/m${i}.svg?react`))
            }

            for (let i = 0; i < 35; i++) {
                pointCards.push(import(`./assets/point-cards/p${i}.svg?react`))
            }

            setAssets([
              (await Promise.all(pointCards)).map(m => m.default),
              (await Promise.all(merchantCards)).map(m => m.default)
            ])
        }

        loadAssets()
    }, [])

    return <AssetsContext.Provider value={assets}>{children}</AssetsContext.Provider>
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
    <AssetsProvider>
      {page === Page.Home && userName && (
        <Home userName={userName} onPlay={onPlay} />
      )}
      {page === Page.Lobby && lobby && (
        <Lobby userId={userId} userName={userName} {...lobby} onStart={onStart} />
      )}
      {page === Page.Game && userId !== -1 && game && (
        <Game
          userId={userId}
          {...game}
          onHome={onHome}
        />
      )}
    </AssetsProvider>
    // <AssetsProvider>
    //   <Game
    //     userId={mockGame.players[1].id}
    //     {...mockGame}
    //     onHome={() => {}}
    //   />
    // </AssetsProvider>
  )
}

export default App
