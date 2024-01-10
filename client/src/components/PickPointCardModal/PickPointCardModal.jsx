import PointCard from "../PointCard/PointCard";
import { IoMdClose } from "react-icons/io";
import './PickPointCardModal.css'
import pointCards from '../../models/PointCards.json'
import { claimMove } from "../../clientMessage";


export default function PickPointCardModal({id, crystals, onClose}) {
    const pointCard = pointCards[id]
    const canClaim = crystals.every((crystal, i) => crystal >= pointCard.crystals[i])
    console.log(canClaim, crystals, pointCard)
    function onClaim() {
        if (!canClaim) {
            return
        }

        claimMove(id)
    }

    return (
        <div className="pick-point-card-modal modal">
            <div className="exit" onClick={onClose}>
                <IoMdClose />
            </div>
            <PointCard id={id} />
            <div className="controls">
                <button className={`${!canClaim ? 'used' : ''}`} onClick={onClaim}>Claim</button>
            </div>
        </div>
    )
}
