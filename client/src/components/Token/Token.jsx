import './Token.css'

export default function Token({numTokens, color}) {
    const colors = ['#cd7f32', 'silver']

    return (
        <div className="token" style={{backgroundColor: colors[color]}}>
            <div>
                {numTokens}
            </div>
        </div>
    )
}
