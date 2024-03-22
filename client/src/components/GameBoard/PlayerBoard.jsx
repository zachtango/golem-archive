import CopperToken from '../Tokens/CopperToken'
import CrystalCount from '../CrystalCount/CrystalCount'
import MerchantCard from '../Cards/MerchantCard'
import PointCard from '../Cards/PointCard'
import SilverToken from '../Tokens/SilverToken'
import PickupCard from '../../assets/pickupcard.svg?react'
import PointCardCover from '../../assets/point-cards/point-card-cover.svg?react'
import MerchantCardCover from '../../assets/merchant-cards/merchant-card-cover.svg?react'
import './PlayerBoard.css'


export default function PlayerBoard({
    isOwnPlayer,
    crystals, numSilverTokens, numCopperTokens,
    pointCardIds, merchantCardIds, usedMerchantCardIds,
    onPlayerMerchantCardClick
}) {

    return (
        <div className='player-board'>
            <div className='head'>
                <div className='sub-head'>
                    <CrystalCount crystals={crystals} />
                    <div className='tokens'>
                        <SilverToken
                            numTokens={numSilverTokens}
                        />
                        <CopperToken
                            numTokens={numCopperTokens}
                        />
                    </div>
                    <div className='point-cards'>
                        {pointCardIds.map((id) => (
                            isOwnPlayer ? (
                                <PointCard id={id} />
                            ) : (
                                <div className='card'>
                                    <PointCardCover />
                                </div>
                            )
                        ))}
                    </div>
                </div>
                <div
                    className='rest-button'
                    style={{
                        opacity: isOwnPlayer ? 1 : 0.5
                    }}
                >
                    <PickupCard />
                </div>
            </div>
            <div className='merchant-cards'>
                {merchantCardIds.map((id) => {
                    const used = usedMerchantCardIds.includes(id)

                    const card = isOwnPlayer || used ? (
                        <MerchantCard
                            id={id}
                            used={used}
                            onClick={isOwnPlayer && !used && (() => onPlayerMerchantCardClick(id))}
                        />
                    ) : (
                        <div className='card'>
                            <MerchantCardCover />
                        </div>
                    )

                    return card
                })}
            </div>
        </div>
    )
}