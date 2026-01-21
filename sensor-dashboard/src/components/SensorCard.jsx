import React from 'react';

const SensorCard = ({ title, value, unit, icon: Icon, color, status, isTemperature }) => {
    const isWarning = status === 'warning';
    const isCritical = status === 'critical';

    const getStatusColor = () => {
        if (isCritical) return 'var(--color-danger)';
        if (isWarning) return 'var(--color-warning)';
        return 'var(--color-success)';
    };

    const getStatusText = () => {
        if (isTemperature && value) {
            if (value < 30.0) return 'Ambient Temperature';
            if (value >= 35.0 && value <= 38.0) return 'Body Temperature - Normal';
            if (value > 38.0) return 'High Temperature!';
            return 'Measuring...';
        }
        return status === 'normal' ? 'Normal' : status === 'warning' ? 'Warning' : 'Critical';
    };

    return (
        <div style={{
            backgroundColor: 'var(--color-surface)',
            borderRadius: 'var(--radius-md)',
            padding: 'var(--spacing-lg)',
            boxShadow: 'var(--shadow-md)',
            display: 'flex',
            flexDirection: 'column',
            gap: 'var(--spacing-md)',
            transition: 'transform 0.2s ease',
            border: isCritical ? `2px solid var(--color-danger)` : '1px solid transparent'
        }}>
            <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
                <h3 style={{ fontSize: '1rem', color: 'var(--color-text-secondary)', fontWeight: 500, textTransform: 'uppercase', letterSpacing: '0.5px' }}>
                    {title}
                </h3>
                <div style={{
                    backgroundColor: isCritical ? 'rgba(255, 59, 48, 0.1)' : 'var(--color-primary-light)',
                    padding: '8px',
                    borderRadius: '12px',
                    color: isCritical ? 'var(--color-danger)' : color || 'var(--color-primary)'
                }}>
                    {Icon && <Icon size={24} />}
                </div>
            </div>

            <div style={{ display: 'flex', alignItems: 'baseline', gap: '4px' }}>
                <span style={{
                    fontSize: '3rem',
                    fontWeight: 700,
                    color: isCritical ? 'var(--color-danger)' : 'var(--color-text-primary)'
                }}>
                    {value || 0}
                </span>
                <span style={{ fontSize: '1.25rem', color: 'var(--color-text-secondary)', fontWeight: 500 }}>
                    {unit}
                </span>
            </div>

            <div style={{ display: 'flex', alignItems: 'center', gap: '8px', marginTop: 'auto' }}>
                <div style={{
                    height: '6px',
                    flex: 1,
                    backgroundColor: 'var(--color-background)',
                    borderRadius: '4px',
                    overflow: 'hidden'
                }}>
                    <div style={{
                        height: '100%',
                        width: '100%',
                        backgroundColor: getStatusColor(),
                        opacity: 0.8
                    }}></div>
                </div>
                <span style={{ fontSize: '0.875rem', color: getStatusColor(), fontWeight: 600 }}>
                    {getStatusText()}
                </span>
            </div>
        </div>
    );
};

export default SensorCard;
