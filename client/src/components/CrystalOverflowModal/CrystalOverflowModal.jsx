import { IoMdClose } from 'react-icons/io'
import { FaArrowDown } from 'react-icons/fa'
import './CrystalOverflowModal.css'
import Crystals from '../Crystals/Crystals'
import { useState } from 'react'
import { removeCrystalOverflow } from '../../clientMessage'


export default function CrystalOverflowModal({crystals}) {
    const [toCrystals, setToCrystals] = useState(crystals)

    function onFromCrystalsClick(crystal) {
        if (toCrystals[crystal] === 0) {
            return
        }
        const newToCrystals = toCrystals.slice()
        newToCrystals[crystal] -= 1

        setToCrystals(newToCrystals)
    }

    function onToCrystalClick(crystal) {
        if (toCrystals[crystal] >= crystals[crystal]) {
            return
        }
        const newToCrystals = toCrystals.slice()
        newToCrystals[crystal] += 1

        setToCrystals(newToCrystals)
    }

    const canDrop = toCrystals.reduce((sum, crystals) => (sum + crystals), 0) === 10

    function onOkay() {
        if (!canDrop) {
            return
        }
        
        removeCrystalOverflow(toCrystals)
    }

    return (
        <div className="crystal-overflow-modal modal">
            <p>You have over 10 crystals</p>
            <div className='crystals-container'>
                <Crystals
                    crystals={crystals}
                    onCrystalClick={onFromCrystalsClick}    
                />
                <FaArrowDown />
                {toCrystals && (
                    <Crystals
                        crystals={toCrystals}
                        onCrystalClick={onToCrystalClick}
                    />
                )}
            </div>
            {canDrop && <button onClick={onOkay}>Okay</button>}
        </div>
    )
}