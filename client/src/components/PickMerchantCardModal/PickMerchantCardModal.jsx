import MerchantCard from '../MerchantCard/MerchantCard'
import { IoMdClose } from "react-icons/io";
import './PickMerchantCardModal.css'


export default function PickMerchantCardModal({id, onClose}) {
    return (
        <div className="pick-merchant-card-modal modal">
            <div className="exit" onClick={onClose}>
                <IoMdClose />
            </div>
            <MerchantCard id={id} />
            <div className="controls">
                <button>Acquire</button>
            </div>
        </div>
    )
}