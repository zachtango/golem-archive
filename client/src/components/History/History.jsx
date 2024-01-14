import './History.css'


export default function History({history}) {

    return (
        <div className="history">
            {history.slice().reverse().map(action => (
                <div className='action'>
                    {action}
                </div>
            ))}
        </div>
    )
}
