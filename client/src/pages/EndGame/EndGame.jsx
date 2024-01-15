import Crystal from '../../components/Crystals/Crystal'
import PlayerStatsRow from '../../components/PlayerStatsRow/PlayerStatsRow'
import Token from '../../components/Token/Token'
import pointCards from '../../models/PointCards.json'
import './EndGame.css'

function getTotalPoints(player) {
    const {pointCardIds, crystals, numSilverTokens, numCopperTokens} = player

    const points = (
        pointCardIds.reduce((sum, id) => sum + pointCards[id].numPoints, 0) +
        crystals.reduce((sum, crystals, i) =>  sum + (i > 0 ? crystals : 0), 0) + // only add crystals if they're not yellows
        numSilverTokens +
        numCopperTokens * 3
    )

    return points
}

function sortPlayers(players) {
    players.sort((p1, p2) => {
        const points1 = getTotalPoints(p1)
        const points2 = getTotalPoints(p2)

        if (points1 < points2) {
            return 1
        } else if (points1 === points2) {
            return 0
        }

        return -1
    })

    return players
}

const places = ['1st', '2nd', '3rd', '4th', '5th', '6th']

export default function EndGame({userId, players, onHome}) {
    
    const sortedPlayers = sortPlayers(players)
    const index = sortedPlayers.indexOf(sortedPlayers.find(p => p.id === userId))

    return (
        <div className="end-game">
            <h1>You are {places[index]}!</h1>

            <div className='head'>
                <div />
                <div>
                    Points
                </div>
                <div className='point-cards'>
                    Point Cards
                </div>
                <div>
                    <Token color={0} />
                </div>
                <div>
                    <Token color={1} />
                </div>
                <div>
                    <Crystal color={1} />
                </div>
                <div>
                    <Crystal color={2} />
                </div>
                <div>
                    <Crystal color={3} />
                </div>
            </div>
            <div className='player-stats'>
                {sortedPlayers.map(player => {
                    return (
                        <PlayerStatsRow
                            key={player.id}
                            points={getTotalPoints(player)}
                            {...player}
                        />
                    )
                })}
            </div>

            <button onClick={onHome}>Home</button>
        </div>
    )
}
