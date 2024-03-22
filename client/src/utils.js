import pointCards from './models/PointCards.json'

export function getPoints(player) {
    
    return (
        player['numSilverTokens'] +
        3 * player['numCopperTokens'] +
        player['pointCardIds'].map(id => pointCards[id]['numPoints']).reduce((prev, curr) => prev + curr, 0) +
        player['crystals'][1] + 
        player['crystals'][2] +
        player['crystals'][3]
    )

}