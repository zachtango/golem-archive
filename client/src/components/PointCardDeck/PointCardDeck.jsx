import './PointCardDeck.css'


export default function PointCardDeck({numPointCards}) {

    return (
        <div className="point-card-deck deck">
            {numPointCards}
        </div>
    )
}