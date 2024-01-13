import Crystals from "../Crystals/Crystals";
import MerchantCard from "../MerchantCard/MerchantCard";
import PointCard from "../PointCard/PointCard";
import Token from "../Token/Token";
import { IoIosRefresh } from "react-icons/io";
import './PlayerBoard.css'
import { restMove } from "../../clientMessage";


export default function PlayerBoard({
    id,
    crystals,
    merchantCardIds,
    usedMerchantCardIds,
    pointCardIds,
    numCopperTokens,
    numSilverTokens,
    onMerchantCardClick,
}) {

    const canRest = usedMerchantCardIds.length > 0

    function onRest() {
        if (!canRest) {
            return
        }

        restMove()
    }

    let cardHeightPercentage = 100

    if (merchantCardIds.length > 17) {
        cardHeightPercentage = 50
    } else if (merchantCardIds.length > 14) {
        cardHeightPercentage = 60
    } else if (merchantCardIds.length > 11) {
        cardHeightPercentage = 80
    }


    return (
        <div className='player-board'>
            <div className='head'>
                <div className='crystals-container'>
                    <Crystals
                        crystals={crystals}
                    />
                </div>
                <div className='token-container'>
                    <div className='silver-token'>
                        <Token numTokens={numSilverTokens} color={1} />
                    </div>
                </div>
                <div className='token-container'>
                    <div className='copper-token'>
                        <Token numTokens={numCopperTokens} color={0} />
                    </div>
                </div>
                <div className='point-cards-container'>
                    <div className='point-cards'>
                        {pointCardIds.map(id => (
                            <PointCard key={id} id={id} />
                        ))}
                    </div>
                </div>
                {/* <div className='turn-status-container' style={{display: 'none'}}>
                    <div className='turn-status'>P1's turn...</div>
                </div> */}
            </div>
            <div className='body'>
                <div className='merchant-cards-container'>
                    <div className='merchant-cards'>
                        {merchantCardIds.map(id => {
                            const used = usedMerchantCardIds.includes(id)
                            return (
                                <div
                                    key={id}
                                    className="merchant-card-container"
                                    style={{height: `${cardHeightPercentage}%`}}
                                >
                                    <MerchantCard
                                        id={id}
                                        used={used}
                                        onClick={!used && (() => onMerchantCardClick(id))}
                                    />
                                </div>
                            )
                        })}
                    </div>
                </div>
                <div className='rest-container'>
                    <div className={`rest ${!canRest ? 'used' : ''}`} onClick={onRest}>
                        <IoIosRefresh />
                    </div>
                </div>
            </div>
        </div>
    )
}