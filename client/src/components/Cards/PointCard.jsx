import React, { useState, useEffect } from 'react';

export default function PointCard({ id, onClick=() => {} }) {
    const [Card, setCard] = useState(null);

    useEffect(() => {
        const importCard = async () => {
            try {
                const module = await import(`../../assets/point-cards/p${id}.svg?react`);
                setCard(module.default);

            } catch (error) {
                console.error('Error loading SVG:', error);
            }
        };

        importCard();

        // Cleanup function if needed
        return () => {
            setCard(null);
        };
    }, [id]);

    return (
        <div className='card' onClick={onClick}>
            {Card}
        </div>
    );
}
