import MerchantCard from "../MerchantCard/MerchantCard"
import MerchantCardDeck from '../MerchantCardDeck/MerchantCardDeck'
import MerchantCardCrystals from "../MerchantCardCrystals/MerchantCardCrystals"
import PointCard from "../PointCard/PointCard"
import PointCardDeck from "../PointCardDeck/PointCardDeck"
import Token from "../Token/Token"
import './GameBoard.css'


export default function GameBoard({onPointCardClick, onMerchantCardClick}) {
    const merchantCardIds = [0, 1, 2, 3, 4, 5]

    const tokens = [6, 6]
    const pointCardIds = [0, 1, 2, 3, 4]

    return (
        <div className='game-board'>
            <div className='point-cards'>
                <div>
                    <div />
                    <PointCardDeck
                        numPointCards={10}
                    />
                </div>

                {pointCardIds.slice().reverse().map((id, i) => (
                    <div
                        key={id}
                        onClick ={() => onPointCardClick(id)}
                    >
                        {i > 2 ? <Token numTokens={tokens[4 - i]} color={4 - i} /> : <div />}
                        <PointCard id={id} />
                    </div>
                ))}
            </div>
            <div className='merchant-cards'>
                <div>
                    <MerchantCardDeck numMerchantCards={10} />
                    <MerchantCardCrystals crystals={[]} />
                </div>
                {merchantCardIds.slice().reverse().map(id => (
                    <div
                        key={id}
                        onClick={() => onMerchantCardClick(id)}
                    >
                        <MerchantCard id={id} />
                        <MerchantCardCrystals crystals={[0, 0]} />
                    </div>
                ))}
            </div>
        </div>
    )
}