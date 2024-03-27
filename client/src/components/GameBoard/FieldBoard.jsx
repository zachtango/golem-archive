import CopperToken from '../Tokens/CopperToken'
import CrystalDisplay from '../CrystalDisplay/CrystalDisplay'
import SilverToken from '../Tokens/SilverToken'
import PointCard from '../Cards/PointCard'
import MerchantCard from '../Cards/MerchantCard'
import PointCardCover from '../../assets/point-cards/point-card-cover.svg?react'
import MerchantCardCover from '../../assets/merchant-cards/merchant-card-cover.svg?react'
import './FieldBoard.css'


export default function FieldBoard({activePointCardIds, activeMerchantCardIds, numSilverTokens, numCopperTokens, fieldCrystals,
    onActiveMerchantCardClick, onActivePointCardClick
}) {

    return (
        <div className='field-board'>
            <div className='point-cards'>
                <div className='tokens'>
                    <SilverToken numTokens={numSilverTokens} />
                    {numCopperTokens > 0 && <CopperToken numTokens={numCopperTokens} />}
                </div>
                <div className='cards'>
                    <div className='card'>
                        <PointCardCover />
                    </div>
                    {activePointCardIds.map((id) => (
                        <PointCard
                            key={id}
                            id={id}
                            onClick={() => onActivePointCardClick(id)}
                        />
                    ))}
                </div>
            </div>
            <div className='merchant-cards'>
                <div className='cards'>
                    <div className='card'>
                        <MerchantCardCover />
                    </div>
                    {activeMerchantCardIds.map((id) => (
                        <MerchantCard
                            key={id}
                            id={id}
                            onClick={() => onActiveMerchantCardClick(id)}
                        />
                    ))}
                </div>
                <div className='crystals'>
                    {fieldCrystals.map((crystals, i) => (
                        <CrystalDisplay
                            key={i}
                            crystals={crystals}
                        />
                    ))}
                </div>
            </div>
        </div>
    )
}