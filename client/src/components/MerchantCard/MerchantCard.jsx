import './MerchantCard.css'

export default function MerchantCard({id, used, onClick}) {

    return (
        <div className={`merchant-card card ${used ? "used" : ""}`} onClick={onClick}>
            <img src={`/assets/m${id}.svg`} />
        </div>
    )
}
