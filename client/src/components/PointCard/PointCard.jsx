import Crystal from '../Crystals/Crystal'
import './PointCard.css'
import pointCards from '../../models/PointCards.json'

export default function PointCard({id}) {
    const pointCard = pointCards[id]

    const yellows = Array.from({ length: pointCard.crystals[0] }, (_, i) => (
        <Crystal key={i} color={0} />
    ));

    const greens = Array.from({ length: pointCard.crystals[1] }, (_, i) => (
        <Crystal key={i} color={1} />
    ));

    const blues = Array.from({ length: pointCard.crystals[2] }, (_, i) => (
        <Crystal key={i} color={2} />
    ));

    const pinks = Array.from({ length: pointCard.crystals[3] }, (_, i) => (
        <Crystal key={i} color={2} />
    ));

    return (
        <div className='point-card card'>
            <div>
                {pointCard.numPoints}
            </div>
            <div className='point-card-crystals'>
                {yellows}
                {greens}
                {blues}
                {pinks}
            </div>
        </div>
    )
}
