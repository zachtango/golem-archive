import MerchantCard from '../MerchantCard/MerchantCard'
import { IoMdClose } from "react-icons/io";
import './PickMerchantCardModal.css'
import { acquireMove } from '../../clientMessage';
import Crystal from '../Crystals/Crystal';


export default function PickMerchantCardModal({id, position, crystals, onClose}) {
    
    const dropCrystals = [
        ...new Array(crystals[0]).fill([0, 0]),
        ...new Array(crystals[1]).fill([1, 0]),
        ...new Array(crystals[2]).fill([2, 0]),
        ...new Array(crystals[3]).fill([3, 0])
    ].slice(0, position)

    const canAcquire = dropCrystals.length >= position;

    if (!canAcquire) {
        const numMissingCrystals = position - dropCrystals.length
        for (let i = 0; i < numMissingCrystals; i++) {
            dropCrystals.push([0, 1])
        }
    }

    function onAcquire() {
        if (!canAcquire) {
            return
        }

        acquireMove(id, dropCrystals)
    }

    return (
        <div className="pick-merchant-card-modal modal">
            <div className="exit" onClick={onClose}>
                <IoMdClose />
            </div>
            <MerchantCard id={id} />
            <div className="controls">
                <div className='pick-merchant-card-modal-crystals'>
                    {dropCrystals.map(([crystal, used], i) => (
                        <Crystal
                            key={i}
                            color={crystal}
                            used={used}
                        />
                    ))}
                </div>
                <button className={`${!canAcquire ? 'used' : ''}`} onClick={onAcquire}>Acquire</button>
            </div>
        </div>
    )
}