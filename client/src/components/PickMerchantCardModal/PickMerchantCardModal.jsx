import { IoMdClose } from "react-icons/io";
import './PickMerchantCardModal.css'
import MerchantCard from "../Cards/MerchantCard";
import CrystalDisplay from "../CrystalDisplay/CrystalDisplay";
import { acquireMove } from "../../clientMessage";


export default function PickMerchantCardModal({id, position, crystals, onClose}) {
    const canPick = position <= crystals.reduce((prev, curr) => prev + curr, 0)
    const yellows = Math.min(position, crystals[0])
    const greens = Math.min(Math.max(position - crystals[0], 0), crystals[1])
    const blues =  Math.min(Math.max(position - crystals[0] - crystals[1], 0), crystals[2])
    const pinks = Math.min(Math.max(position - crystals[0] - crystals[1] - crystals[2], 0), crystals[3])
    const dropCrystals = [
        ...Array(yellows).fill(0),
        ...Array(greens).fill(1),
        ...Array(blues).fill(2),
        ...Array(pinks).fill(3)
    ]
    console.log(dropCrystals)

    return (
        <div className="pick-merchant-card-modal modal center">
            <div className="exit" onClick={onClose}>
                <IoMdClose />
            </div>
            <MerchantCard id={id} />
            <div className="controls">
                <CrystalDisplay
                    crystals={[yellows, greens, blues, pinks]}
                    totalCrystals={position}
                />
                <button
                    onClick={() => {
                        acquireMove(id, dropCrystals)
                        onClose()
                    }}
                    disabled={!canPick}
                >Acquire</button>
            </div>
        </div>
    )
}
