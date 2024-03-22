import { IoMdClose } from "react-icons/io";
import PointCard from "../Cards/PointCard";
import pointCards from '../../models/PointCards.json'
import { claimMove } from "../../clientMessage";


export default function PickPointCardModal({id, crystals, onClose}) {
    const pointCard = pointCards[id]
    const canClaim = crystals.every((crystal, i) => crystal >= pointCard.crystals[i])

    function onClaim() {
        claimMove(id)
        onClose()
    }

    return (
        <div className="pick-merchant-card-modal modal center">
            <div className="exit" onClick={onClose}>
                <IoMdClose />
            </div>
            <PointCard id={id} />
            <div className="controls">
                <button
                    disabled={!canClaim}
                    onClick={onClaim}
                >Buy</button>
            </div>
        </div>
    )
}
