import { getPoints } from "../../utils"
import PointCard from "../Cards/PointCard"
import CrystalCount from "../CrystalCount/CrystalCount"
import CopperToken from "../Tokens/CopperToken"
import SilverToken from "../Tokens/SilverToken"
import './EndStats.css'


export default function EndStats({ownPlayer, players}) {
    
    const sortedPlayers = players.slice()
    
    sortedPlayers.sort((p1, p2) => {
        const points1 = getPoints(p1)
        const points2 = getPoints(p2)

        return points1 < points2 ? 1 : points1 === points2 ? 0 : -1
    })

    console.log(sortedPlayers)
    return (
        <div className='end-stats center'>
            <h1>{sortedPlayers[0]['userName']} Wins!</h1>
            {sortedPlayers.map(player => (
                <div
                    key={player['id']}
                    className='end-stats-player'
                >
                    <div className='head'>
                        <div className='name'>{player['userName']}</div>
                        <div className='points'>{getPoints(player)} points</div>
                    </div>
                    <div className='point-cards'>
                        {player['pointCardIds'].map(id => (
                            <PointCard id={id} />
                        ))}
                    </div>
                </div>
            ))}
        </div>
    )
}