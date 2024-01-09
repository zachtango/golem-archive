import { useEffect, useState } from 'react'
import './reset.css'
import './App.css'
import Home from './pages/Home'
import Lobby from './pages/Lobby'
import Game from './components/Game/Game'

const Page = {
  Home: 0,
  Lobby: 1,
  Game: 2
}

function App() {
  const [page, setPage] = useState(Page.Home)

  let content;
  switch(page) {
    case Page.Home:
      content = (
        <Home
          onPlay={() => setPage(Page.Lobby)}
        />
      )
      break
    case Page.Lobby:
      content = (
        <Lobby
          onStart={() => setPage(Page.Game)}
        />
      )
      break
    case Page.Game:
      content = <Game />
      break
  }


  return content
}

export default App
