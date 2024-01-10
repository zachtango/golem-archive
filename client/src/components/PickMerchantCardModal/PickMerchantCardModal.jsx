import MerchantCard from '../MerchantCard/MerchantCard'
import { IoMdClose } from "react-icons/io";
import './PickMerchantCardModal.css'
import MerchantCardCrystals from '../MerchantCardCrystals/MerchantCardCrystals';
import { acquireMove } from '../../clientMessage';


export default function PickMerchantCardModal({id, position, crystals, onClose}) {
    
    const dropCrystals = [
        ...new Array(crystals[0]).fill(0),
        ...new Array(crystals[1]).fill(1),
        ...new Array(crystals[2]).fill(2),
        ...new Array(crystals[3]).fill(3),
    ].slice(0, position)

    const canAcquire = dropCrystals.length >= position;

    function onAcquire() {
        if (!canAcquire) {
            return
        }
        console.log('Acquire ', id, dropCrystals)
        acquireMove(id, dropCrystals)
    }

    return (
        <div className="pick-merchant-card-modal modal">
            <div className="exit" onClick={onClose}>
                <IoMdClose />
            </div>
            <MerchantCard id={id} />
            <div className="controls">
                <MerchantCardCrystals crystals={dropCrystals} />
                <button className={`${!canAcquire ? 'used' : ''}`} onClick={onAcquire}>Acquire</button>
            </div>
        </div>
    )
}