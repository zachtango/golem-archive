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
    onPointCardClick,
    onMerchantCardClick
}) {
    const tokens = [numSilverTokens, numCopperTokens]

    return (
        <div className='game-board'>
            <div className='point-cards'>
                <div>
                    <div />
                    <PointCardDeck
                        numPointCards={10}
                    />
                </div>

                {activePointCardIds.slice().reverse().map((id, i) => (
                    <div
                        key={id}
                        onClick ={() => onPointCardClick(id)}
                    >
                        {i > 2 ? <Token numTokens={tokens[i - 3]} color={4 - i} /> : <div />}
                        <PointCard id={id} />
                    </div>
                ))}
            </div>
            <div className='merchant-cards'>
                <div>
                    <MerchantCardDeck numMerchantCards={10} />
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