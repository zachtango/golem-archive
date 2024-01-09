import Crystal from '../Crystals/Crystal'
import './MerchantCardCrystals.css'


export default function MerchantCardCrystals({crystals}) {

    return (
        <div className='merchant-card-crystals'>
            {crystals.map((crystal, i) => (
                <Crystal
                    key={i}
                    color={crystal}
                />
            ))}
        </div>
    )
}
