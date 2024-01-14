import './PlayerStatsRow.css'
import pointCards from '../../models/PointCards.json'
import PointCard from '../PointCard/PointCard'


export default function PlayerStatsRow({
    points,
    pointCardIds,
    id,
    crystals,
    numSilverTokens,
    numCopperTokens
}) {

    return (
        <div className="player-stats-row">
            <div>
                P{id}
            </div>
            <div>
                {points}
            </div>
            <div className='point-cards'>
                {pointCardIds.map(id => (
                    <PointCard
                        key={id}
                        id={id}
                    />
                ))}
            </div>
            <div>
                {numCopperTokens}
            </div>
            <div>
                {numSilverTokens}
            </div>
            <div>
                {crystals[1]}
            </div>
            <div>
                {crystals[2]}
            </div>
            <div>
                {crystals[3]}
            </div>
        </div>
    )
}
