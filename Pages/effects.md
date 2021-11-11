# Effects
Cameron Hurley, component owner
- 4 Effects
- Component Passes Audio
- Controllable Effects Sends

## Chorus
An effect which adds previous input from a range around a short delay to create a multi-voice unison effect. 
### Parameters:
- `delay`: how many seconds the largest delay is from the actual time on average (10ms is good)
- `wet`: what proportion of the effect output to use (0-1)
- `dry`: what proportion of the base input to use (0-1, which should be the 1 - `wet`)
- `range`: the proportion over which `delay` should vary. Ex: `range=".1"` will cause the delay to oscillate 10% of `delay` from `delay` (0-1)
- `rate`: the frequency the delay should oscillate over its range by (<20Hz)

## Flange
An effect which adds swept delayed input with the current input.
### Parameters:
- `delay`: how many seconds the largest delay is from the actual time on average (10ms is good)
- `wet`: what proportion of the effect output to use (0-1)
- `dry`: what proportion of the base input to use (0-1, which should be the 1 - `wet`)
- `range`: the proportion over which `delay` should vary. Ex: `range=".1"` will cause the delay to oscillate 10% of `delay` from `delay` (0-1)
- `rate`: the frequency the delay should oscillate over its range by (<20Hz)
- `num`: the number of filters the input will go through (0 or more)

## Limiter
An effect which reduces the volume of loud sounds or amplifies quiet sounds.

### Parameters:
- `threshold`: the range of signals that can be used and the level at which the gate will open
- `wet`: what proportion of the effect output to use (0-1)
- `dry`: what proportion of the base input to use (0-1, which should be the 1 - `wet`)
- `ratio`: the amount of gain reduction in the sound
	
## Noise Gate
An effect which controls the volume of an audio signal.

### Parameters:
- `threshold`: the range of signals that can be used and the level at which the gate will open
- `wet`: what proportion of the effect output to use (0-1)
- `dry`: what proportion of the base input to use (0-1, which should be the 1 - `wet`)

## Example
*Many Meetings* by Howard Shore (arranged and used from a *Lord of the Rings* clarinet book).

Each measure using a different effect: chorus, flange, limiter, noise gate, then the rest of the phrase with no effects.

[Score File](/Scores/effects-example.score)

[Audio File](/Scores/effects-example.wav)