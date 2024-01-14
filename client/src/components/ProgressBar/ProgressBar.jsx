import './ProgressBar.css'

export default function ProgressBar({numGolems, numGolemsToEnd}) {

    const segments = Array.from({ length: numGolems }, (_, i) => (
        <div
            key={i}
            className='segment'
            style={{
                width: `${100 / numGolemsToEnd}%`
            }}
        />
    ));

    return (
        <div className="progress-bar">
            <div className='text'>
                {numGolems} / {numGolemsToEnd}
            </div>
            {segments}
        </div>
    )
}
