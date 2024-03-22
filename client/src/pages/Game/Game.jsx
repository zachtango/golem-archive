import './Game.css'
import Logo from '../../assets/logo.svg?react'
import GameBoard from '../../components/GameBoard/GameBoard'
import Chat from '../../components/Chat/Chat'
import ProgressBar from '../../components/ProgressBar/ProgressBar'
import PickMerchantCardModal from '../../components/PickMerchantCardModal/PickMerchantCardModal'
import PickPointCardModal from '../../components/PickPointCardModal/PickPointCardModal'
import PlayMerchantCardModal from '../../components/PlayMerchantCardModal/PlayMerchantCardModal'
import ScoreBoard from '../../components/ScoreBoard/ScoreBoard'
import { useState } from 'react'

export default function Game({
    userId,
    turn, round, maxGolems,
    activePointCardIds, activeMerchantCardIds,
    fieldCrystals, numSilverTokens, numCopperTokens,
    players, chat
}) {
    const ownPlayer = players.find(player => player['id'] === userId)

    const [player, setPlayer] = useState(ownPlayer)
    const [pickMerchantCardId, setPickMerchantCardId] = useState(-1)
    const [pickPointCardId, setPickPointCardId] = useState(-1)
    const [playMerchantCardId, setPlayMerchantCardId] = useState(-1)

    function resetModals() {
        setPickMerchantCardId(-1)
        setPickPointCardId(-1)
        setPlayMerchantCardId(-1)
    }
        
    return (
        <div className='game page'>
            <div className='game-wrapper'>
                {pickMerchantCardId !== -1 && (
                    <PickMerchantCardModal
                        id={pickMerchantCardId}
                        position={activeMerchantCardIds.length - activeMerchantCardIds.indexOf(pickMerchantCardId) - 1}
                        crystals={ownPlayer['crystals']}
                        onClose={() => setPickMerchantCardId(-1)}
                    />
                )}
                {pickPointCardId !== -1 && (
                    <PickPointCardModal
                        id={pickPointCardId}
                        onClose={() => setPickPointCardId(-1)}
                    />
                )}
                {playMerchantCardId !== -1 && (
                    <PlayMerchantCardModal
                        id={playMerchantCardId}
                        crystals={ownPlayer['crystals']}
                        onClose={() => setPlayMerchantCardId(-1)}
                    />
                )}

                <div className='head'>
                    <Logo className='logo' />
                    <div className='status'>
                        <div className='round'>
                            Round {round}
                        </div>
                        <div className='golems'>
                            <div>Golems to end: {maxGolems}</div>
                            <ProgressBar
                                complete={Math.max(...players.map(player => player['pointCardIds'].length))}
                                total={maxGolems}
                            />
                        </div>
                    </div>
                </div>
                <div className='body'>
                    <ScoreBoard
                        ownPlayer={ownPlayer}
                        player={player}
                        players={players}
                        turn={turn}

                        onPlayerClick={player => setPlayer(player)}
                    />
                    <GameBoard
                        activePointCardIds={activePointCardIds}
                        activeMerchantCardIds={activeMerchantCardIds}
                        numSilverTokens={numSilverTokens}
                        numCopperTokens={numCopperTokens}
                        fieldCrystals={fieldCrystals}

                        isOwnPlayer={player['id'] === userId}
                        player={player}

                        onActivePointCardClick={(id) => {
                            resetModals()
                            setPickPointCardId(id)
                        }}
                        onActiveMerchantCardClick={(id) => {
                            resetModals()
                            setPickMerchantCardId(id)
                        }}
                        onPlayerMerchantCardClick={(id) => {
                            resetModals()
                            setPlayMerchantCardId(id)
                        }}
                    />
                    <Chat
                        chat={chat}
                    />
                </div>
            </div>
        </div>
    )
}