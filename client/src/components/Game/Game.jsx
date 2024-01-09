import { useEffect, useState } from 'react'
import './Game.css'
import ProgressBar from '../ProgressBar/ProgressBar'
import GameBoard from '../GameBoard/GameBoard'
import PlayerBoard from '../PlayerBoard/PlayerBoard'
import PlayerRow from '../PlayerRow/PlayerRow'
import PickPointCardModal from '../PickPointCardModal/PickPointCardModal'
import PickMerchantCardModal from '../PickMerchantCardModal/PickMerchantCardModal'
import PlayMerchantCardModal from '../PlayMerchantCardModal/PlayMerchantCardModal'


function Game() {
  const [pickPointCardModalId, setPickPointCardModalId] = useState(-1)
  const [pickMerchantCardModalId, setPickMerchantCardModalId] = useState(-1)
  const [playMerchantCardModalId, setPlayMerchantCardModalId] = useState(-1)

  const players = [0, 1, 2, 3, 4]

  function resetModals() {
    setPickPointCardModalId(-1)
    setPickMerchantCardModalId(-1)
    setPlayMerchantCardModalId(-1)
  }

  function onGameBoardPointCardClick(id) {
    resetModals()
    setPickPointCardModalId(id)
  }
  
  function onGameBoardMerchantCardClick(id) {
    resetModals()
    setPickMerchantCardModalId(id)
  }

  function onPlayerBoardMerchantCardClick(id) {
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

  return (
    <>
      <div className='game'>
        {pickPointCardModalId != -1 && (
          <PickPointCardModal
            id={pickPointCardModalId}
            onClose={resetModals}
          />
        )}
        {pickMerchantCardModalId != -1 && (
          <PickMerchantCardModal
            id={pickMerchantCardModalId}
            onClose={resetModals}
          />
        )}
        {playMerchantCardModalId != -1 && (
          <PlayMerchantCardModal
            id={playMerchantCardModalId}
            onClose={resetModals}  
          />
        )}

        <div className='head'>
          <div>
            Golems
          </div>
          <ProgressBar
            numGolems={4}
            numGolemsToEnd={6}  
          />
        </div>
        <div className='body'>
          <div className='board'>
            <GameBoard
              onPointCardClick={onGameBoardPointCardClick}
              onMerchantCardClick={onGameBoardMerchantCardClick}
            />
            <PlayerBoard
              onMerchantCardClick={onPlayerBoardMerchantCardClick}
            />
          </div>
          <div className='side'>
            {players.map((player, i) => (
              <div 
                key={i}
                className='player-row-container'
              >
                <PlayerRow />
              </div>
            ))}
          </div>
        </div>
      </div>
    </>
  )
}

export default Game
