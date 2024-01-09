import './MerchantCardDeck.css'


export default function MerchantCardDeck({numMerchantCards, used = false}) {

    return (
        <div className={`merchant-card-deck deck ${used ? 'used' : ''}`}>
            {numMerchantCards}
        </div>
    )
}