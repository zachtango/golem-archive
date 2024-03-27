import CopperToken from '../Tokens/CopperToken'
import SilverToken from '../Tokens/SilverToken'
import CrystalCount from '../CrystalCount/CrystalCount'
import './PlayerRow.css'


export default function PlayerRow({
    selected=false,
    id,
    userName,
    turn,
    crystals,
    merchantCardIds,
    usedMerchantCardIds,
    pointCardIds,
    numCopperTokens,
    numSilverTokens,
    active,
    onClick
}) {
    return (
        <div
            className={`player-row ${active ? 'active-player-row' : ''}`}
            onClick={onClick}
            style={{
                backgroundColor: selected ? 'rgb(120, 120, 255, 0.5)' : ''
            }}
        >
            <div className='head'>
                <div className='name'>{userName}</div>
                <div className='desc'>{pointCardIds.length} golems</div>
            </div>
            <CrystalCount crystals={crystals} />
            <div className='tokens'>
                <SilverToken numTokens={numSilverTokens} />
                <CopperToken numTokens={numCopperTokens} />
            </div>
        </div>
    )
}