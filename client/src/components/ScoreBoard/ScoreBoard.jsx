import PlayerRow from './PlayerRow'
import Timer from '../Timer/Timer'
import './ScoreBoard.css'
import { getPoints } from '../../utils'

export default function ScoreBoard({ownPlayer, player, players, turn, onPlayerClick}) {

    const currentPlayer = players.find(player => player.turn === turn)

    return (
        <div className='score-board'>
            <div className='players'>
                <div className='points'>
                    You have {getPoints(ownPlayer)} points
                </div>
                {players.map(p => (
                    <PlayerRow
                        key={p['id']}
                        selected={p['id'] === player['id']}
                        {...p}
                        onClick={() => onPlayerClick(p)}
                    />
                ))}
            </div>
            <Timer
                userName={ownPlayer['id'] === currentPlayer['id'] ? 'Your' : currentPlayer['userName'] + "'s"}
            />
        </div>
    )
}