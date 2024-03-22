import { IoMdClose } from "react-icons/io";
import './PickMerchantCardModal.css'
import MerchantCard from "../Cards/MerchantCard";
import CrystalDisplay from "../CrystalDisplay/CrystalDisplay";


export default function PickMerchantCardModal({id, position, crystals, onClose}) {
    

    return (
        <div className="pick-merchant-card-modal modal center">
            <div className="exit" onClick={onClose}>
                <IoMdClose />
            </div>
            <MerchantCard id={id} />
            <div className="controls">
                <CrystalDisplay
                    crystals={[
                        Math.min(position, crystals[0]),
                        Math.min(Math.max(position - crystals[0], 0), crystals[1]),
                        Math.min(Math.max(position - crystals[0] - crystals[1], 0), crystals[2]),
                        Math.min(Math.max(position - crystals[0] - crystals[1] - crystals[2], 0), crystals[3]),
                    ]}
                    totalCrystals={position}
                />
                <button>Acquire</button>
            </div>
        </div>
    )
}
