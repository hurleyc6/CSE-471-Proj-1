# CSE 471 Project 1
A Multi-Component Music Synthesizer


## Group 14
Anthony Rodeman (rodeman2@msu.edu)
Cameron Hurley (hurleyc6@msu.edu)

## Musical Selection: Title
A short description
[Score File](title.score)
[Audio File](title.wav)

## Effects
Cameron Hurley, component owner
- 3 Effects
- Component Passes Audio
- Controllable Effects Sends

### Chorus
An effect which adds previous input from a range around a short delay to create a multi-voice unison effect. 
#### Parameters:
	- `delay`: how many seconds the largest delay is from the actual time on average (10ms is good)
	- `wet`: what proportion of the effect output to use (0-1)
	- `dry`: what proportion of the base input to use (0-1, which should be the 1 - `wet`)
	- `range`: the proportion over which `delay` should vary. Ex: `range=".1"` will cause the delay to oscillate 10% of `delay` from `delay` (0-1)
	- `rate`: the frequency the delay should oscillate over its range by (<20Hz)

### Flange
An effect which adds swept delayed input with the current input.

### Limiter

### Noise Gate

### Example
*Many Meetings* by Howard Shore (arranged and used from a *Lord of the Rings* clarinet book).
Each measure using a different effect: chorus, flange, limiter, noise gate, then the rest of the phrase with no effects.
[Score File](effects-example.score)
[Audio File](efects-example.wav)

## Additive Synthesizer
Cameron Hurley, component owner

## Wavetable
Anthony Rodeman, component owner
Wavetable using F Horn samples from [Liz Harwood](https://www.youtube.com/watch?v=uUYnIJH5TD0).

### Example
Horn Solo from *You Can't Always Get What You Want* by the Rolling Stones (from a transcription I did for MUS 178 in the Spring)
[Score File](wavetable-example.score)
[Audio File](wavetable-example.wav)

## XML Format
Extension: `.score`
