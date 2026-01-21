import React from 'react';
import { LineChart, Line, ResponsiveContainer, YAxis, XAxis, CartesianGrid, Tooltip } from 'recharts';

const IRGraph = ({ data }) => {
    return (
        <div style={{
            background: 'white',
            borderRadius: '24px',
            padding: 'var(--spacing-lg)',
            boxShadow: 'var(--shadow-lg)',
            border: '1px solid var(--color-border)',
            height: '350px',
            display: 'flex',
            flexDirection: 'column'
        }}>
            <h3 style={{
                color: 'var(--color-text-secondary)',
                fontSize: '1rem',
                fontWeight: '500',
                marginBottom: 'var(--spacing-md)',
                display: 'flex',
                alignItems: 'center',
                gap: '8px'
            }}>
                <span style={{
                    width: '8px',
                    height: '8px',
                    borderRadius: '50%',
                    backgroundColor: '#34C759',
                    display: 'inline-block',
                    boxShadow: '0 0 0 2px rgba(52, 199, 89, 0.2)'
                }}></span>
                IR Signal (Raw)
            </h3>

            <div style={{ flex: 1, width: '100%', minHeight: 0 }}>
                <ResponsiveContainer width="100%" height="100%">
                    <LineChart data={data}>
                        <CartesianGrid strokeDasharray="3 3" stroke="#f0f0f0" vertical={false} />
                        <XAxis dataKey="index" hide />
                        <YAxis
                            hide
                            domain={['auto', 'auto']}
                        />
                        <Tooltip
                            contentStyle={{
                                backgroundColor: 'white',
                                border: 'none',
                                borderRadius: '12px',
                                boxShadow: '0 4px 12px rgba(0,0,0,0.1)'
                            }}
                            itemStyle={{ color: '#34C759', fontWeight: '600' }}
                            cursor={{ stroke: '#34C759', strokeWidth: 1 }}
                            formatter={(value) => [value, 'IR Value']}
                            labelStyle={{ display: 'none' }}
                        />
                        <Line
                            type="monotone"
                            dataKey="value"
                            stroke="#34C759"
                            strokeWidth={3}
                            dot={false}
                            isAnimationActive={false}
                        />
                    </LineChart>
                </ResponsiveContainer>
            </div>
        </div>
    );
};

export default IRGraph;
