import Crystal from './Crystal'
import './Crystals.css'


export default function Crystals({crystals, onCrystalClick=null}) {

    return (
        <div className='crystals'>
            {[0, 1, 2, 3].map((crystal, i) => (
                <div
                    key={i}
                    className={`${crystals[crystal] == 0 ? 'used' : ''}`}
                    onClick={onCrystalClick ? () => onCrystalClick(crystal) : null}
                >
                    <Crystal color={crystal} numCrystals={crystals[i]} />
                </div>
            ))}
        </div>
    )
}
