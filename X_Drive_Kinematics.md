# X-Drive Kinematics Explanation

## Velocity Components
- vx: Forward/Backward velocity
- vy: Left/Right velocity
- ω: Rotational velocity

## Inverse Kinematic Equations
m1 = -vx + vy + ω  
m2 = -vx - vy - ω  
m3 = -vx + vy - ω  
m4 = -vx - vy + ω  

## Normalization
Motor speeds are scaled to ensure values remain within PWM limits (0–255).
