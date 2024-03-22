import { IoMdClose } from "react-icons/io";
import './PickPointCardModal.css'
import PointCard from "../Cards/PointCard";


export default function PickPointCardModal({id, onClose}) {
    // const pointCard = pointCards[id]
    // const canClaim = crystals.every((crystal, i) => crystal >= pointCard.crystals[i])
    // console.log(canClaim, crystals, pointCard)
    // function onClaim() {
    //     if (!canClaim) {
    //         return
    //     }

    //     claimMove(id)
    // }

    return (
        <div className="pick-merchant-card-modal modal center">
            <div className="exit" onClick={onClose}>
                <IoMdClose />
            </div>
            <PointCard id={id} />
            <div className="controls">
                <button>Buy</button>
            </div>
        </div>
    )
}
