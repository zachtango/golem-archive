import MerchantCard from "../MerchantCard/MerchantCard"
import MerchantCardDeck from '../MerchantCardDeck/MerchantCardDeck'
import MerchantCardCrystals from "../MerchantCardCrystals/MerchantCardCrystals"
import PointCard from "../PointCard/PointCard"
import PointCardDeck from "../PointCardDeck/PointCardDeck"
import Token from "../Token/Token"
import './GameBoard.css'


export default function GameBoard({
    activePointCardIds,
    activeMerchantCards,
    numCopperTokens,
    numSilverTokens,
    numPointCards,
    numMerchantCards,
    onPointCardClick,
    onMerchantCardClick
}) {

    return (
        <div className='game-board'>
            <div className='point-cards'>
                <div>
                    <div />
                    <PointCardDeck
                        numPointCards={numPointCards}
                    />
                </div>

                {activePointCardIds.slice().reverse().map((id, i) => {
                    let token = <div />

                    if (i === 3 && numCopperTokens > 0 && numSilverTokens > 0) {
                        token = <Token numTokens={numSilverTokens} color={1} />
                    }
                    
                    if (i === 4 && (numCopperTokens > 0 || numSilverTokens > 0)) {
                        token = numCopperTokens > 0 ? <Token numTokens={numCopperTokens} color={0} /> : <Token numTokens={numSilverTokens} color={1} />
                    }

                    return (
                        <div
                            key={id}
                            onClick ={() => onPointCardClick(id)}
                        >
                            {token}
                            <PointCard id={id} />
                        </div>
                    )
                })}
            </div>
            <div className='merchant-cards'>
                <div>
                    <MerchantCardDeck numMerchantCards={numMerchantCards} />
                    <MerchantCardCrystals crystals={[]} />
                </div>
                {activeMerchantCards.slice().reverse().map(merchantCard => {
                    const crystals = [
                        ...new Array(merchantCard.crystals[0]).fill(0),
                        ...new Array(merchantCard.crystals[1]).fill(1),
                        ...new Array(merchantCard.crystals[2]).fill(2),
                        ...new Array(merchantCard.crystals[3]).fill(3)
                    ]
                    return (
                        <div
                            key={merchantCard.id}
                            onClick={() => onMerchantCardClick(merchantCard.id)}
                        >
                            <MerchantCard id={merchantCard.id} />
                            <MerchantCardCrystals crystals={crystals} />
                        </div>
                    )
                })}
            </div>
        </div>
    )
}