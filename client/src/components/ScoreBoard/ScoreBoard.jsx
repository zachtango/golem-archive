import PlayerRow from './PlayerRow'
import Timer from '../Timer/Timer'
import './ScoreBoard.css'
import { getPoints } from '../../utils'

export default function ScoreBoard({ownPlayer, player, players, turn, onPlayerClick}) {

    return (
        <div className='score-board'>
            <div className='players'>
                <div className='points'>
                    You have {getPoints(ownPlayer)} points
                </div>
                {players.map(p => (
                    <PlayerRow
                        selected={p['id'] === player['id']}
                        {...p}
                        onClick={() => onPlayerClick(p)}
                    />
                ))}
            </div>
            <Timer
                userName={players[turn]['userName']}
                startingSeconds={60 * 1}
            />
        </div>
    )
}