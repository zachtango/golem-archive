import { useEffect, useState } from 'react'
import './Game.css'
import ProgressBar from '../../components/ProgressBar/ProgressBar'
import GameBoard from '../../components/GameBoard/GameBoard'
import PlayerBoard from '../../components/PlayerBoard/PlayerBoard'
import PlayerRow from '../../components/PlayerRow/PlayerRow'
import PickPointCardModal from '../../components/PickPointCardModal/PickPointCardModal'
import PickMerchantCardModal from '../../components/PickMerchantCardModal/PickMerchantCardModal'
import PlayMerchantCardModal from '../../components/PlayMerchantCardModal/PlayMerchantCardModal'
import CrystalOverflowModal from '../../components/CrystalOverflowModal/CrystalOverflowModal'

// Sorts players in accordance to turn with own player anchored at start
function sortPlayers(players, ownPlayerId) {
  players.sort((p1, p2) => {
      return p1['turn'] < p2['turn']
  })
  const k = players.indexOf(players.find(player => player['id'] === ownPlayerId))

  // Rotate array until own player is at start
  for (let i = 0; i < k; i++) {
      players.push(players.shift())
  }

  return players.reverse()
}

function Game({
  userId,
  id,
  turn,
  isDone,
  lastRound,
  maxGolems,
  numCopperTokens,
  numSilverTokens,
  players,
  activeMerchantCards,
  activePointCardIds,
  numPointCards,
  numMerchantCards
}) {
  const ownPlayer = players.find(player => player.id === userId)
  const totalCrystals = ownPlayer.crystals.reduce((sum, crystal) => sum + crystal, 0)
  const activePlayer = players.find(player => player.id === userId)
  const numGolems = players.reduce((numGolems, player) => Math.max(numGolems, player.pointCardIds.length), 0)

  const [pickPointCardModalId, setPickPointCardModalId] = useState(-1)
  const [pickMerchantCardModalId, setPickMerchantCardModalId] = useState(-1)
  const [playMerchantCardModalId, setPlayMerchantCardModalId] = useState(-1)

  function resetModals() {
    setPickPointCardModalId(-1)
    setPickMerchantCardModalId(-1)
    setPlayMerchantCardModalId(-1)
  }

  function onGameBoardPointCardClick(id) {
    if (totalCrystals > 10) {
      return
    }

    resetModals()
    setPickPointCardModalId(id)
  }
  
  function onGameBoardMerchantCardClick(id) {
    if (totalCrystals > 10) {
      return
    }

    resetModals()
    setPickMerchantCardModalId(id)
  }

  function onPlayerBoardMerchantCardClick(id) {
    if (totalCrystals > 10) {
      return
    }

    resetModals()
    setPlayMerchantCardModalId(id)
  }

  useEffect(() => {
    const handleClickOutside = (event) => {
      const isClickInsideModal = event.target.closest('.modal')
      const isClickInsideCard = event.target.closest('.card')

      if (!isClickInsideModal && !isClickInsideCard) {
        resetModals()
      }
    };

    // Add event listener when the component mounts
    document.addEventListener('click', handleClickOutside);

    // Remove event listener when the component unmounts
    return () => {
      document.removeEventListener('click', handleClickOutside);
    };
  }, []);

  useEffect(() => {
    setPickPointCardModalId(-1)
  }, [activePointCardIds])
  
  useEffect(() => {
    setPickMerchantCardModalId(-1)
  }, [activeMerchantCards])
  
  return (
    <>
      <div className='game'>
        {totalCrystals > 10 && (
          <CrystalOverflowModal
            crystals={ownPlayer.crystals}
          />
        )}
        {pickPointCardModalId != -1 && (
          <PickPointCardModal
            id={pickPointCardModalId}
            crystals={ownPlayer.crystals}
            onClose={resetModals}
          />
        )}
        {pickMerchantCardModalId != -1 && (
          <PickMerchantCardModal
            id={pickMerchantCardModalId}
            position={activeMerchantCards.indexOf(activeMerchantCards.find(card => card.id === pickMerchantCardModalId))}
            crystals={ownPlayer.crystals}
            onClose={resetModals}
          />
        )}
        {playMerchantCardModalId != -1 && (
          <PlayMerchantCardModal
            id={playMerchantCardModalId}
            crystals={ownPlayer.crystals}
            onClose={resetModals}
          />
        )}

        <div className='head'>
          <div>
            Golems
          </div>
          <ProgressBar
            numGolems={numGolems}
            numGolemsToEnd={maxGolems}
          />
        </div>
        <div className='body'>
          <div className='board'>
            <GameBoard
              activePointCardIds={activePointCardIds}
              activeMerchantCards={activeMerchantCards}
              numCopperTokens={numCopperTokens}
              numSilverTokens={numSilverTokens}
              numPointCards={numPointCards}
              numMerchantCards={numMerchantCards}
              onPointCardClick={onGameBoardPointCardClick}
              onMerchantCardClick={onGameBoardMerchantCardClick}
            />
            <PlayerBoard
              {...activePlayer}
              onMerchantCardClick={onPlayerBoardMerchantCardClick}
            />
          </div>
          <div className='side'>
            {sortPlayers([...players], userId).map((player) => (
              <div 
                key={player.id}
                className={`player-row-container ${turn !== player.turn ? 'used' : ''}`}
              >
                <PlayerRow {...player} />
              </div>
            ))}
          </div>
        </div>
      </div>
    </>
  )
}

export default Game
