import PointCard from "../PointCard/PointCard";
import { IoMdClose } from "react-icons/io";
import './PickPointCardModal.css'


export default function PickPointCardModal({id, onClose}) {

    return (
        <div className="pick-point-card-modal modal">
            <div className="exit" onClick={onClose}>
                <IoMdClose />
            </div>
            <PointCard id={id} />
            <div className="controls">
                <button>Claim</button>
            </div>
        </div>
    )
}
