# CSE 471 Project 1
A Multi-Component Music Synthesizer


## Group 14
Anthony Rodeman (rodeman2@msu.edu)

Cameron Hurley (hurleyc6@msu.edu)

## Musical Selection: Title
A short description
[Score File](music-selection.score)
[Audio File](music-selection.wav)

## Effects
Cameron Hurley, component owner
- 4 Effects
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
#### Parameters:
	- `delay`: how many seconds the largest delay is from the actual time on average (10ms is good)
	- `wet`: what proportion of the effect output to use (0-1)
	- `dry`: what proportion of the base input to use (0-1, which should be the 1 - `wet`)
	- `range`: the proportion over which `delay` should vary. Ex: `range=".1"` will cause the delay to oscillate 10% of `delay` from `delay` (0-1)
	- `rate`: the frequency the delay should oscillate over its range by (<20Hz)
	- `num`: the number of filters the input will go through (0 or more)

### Limiter
An effect which reduces the volume of loud sounds or amplifies quiet sounds.

#### Parameters:
	- `threshold`: the range of signals that can be used and the level at which the gate will open
	- `wet`: what proportion of the effect output to use (0-1)
	- `dry`: what proportion of the base input to use (0-1, which should be the 1 - `wet`)
	- `ratio`: the amount of gain reduction in the sound
	
### Noise Gate
An effect which controls the volume of an audio signal.

#### Parameters:
	- `threshold`: the range of signals that can be used and the level at which the gate will open
	- `wet`: what proportion of the effect output to use (0-1)
	- `dry`: what proportion of the base input to use (0-1, which should be the 1 - `wet`)

### Example
*Many Meetings* by Howard Shore (arranged and used from a *Lord of the Rings* clarinet book).

Each measure using a different effect: chorus, flange, limiter, noise gate, then the rest of the phrase with no effects.

[Score File](effects-example.score)

[Audio File](effects-example.wav)

## Additive Synthesizer
Cameron Hurley, component owner
- Sinusoid playback on demand from the sequencer
- Definition of all harmonics
- Envelope generation
- Polyphony
- Sound cross-fading
- Vibrato OR glissando

### Parameters:
	- `measure`: the time the notes are played
	- `beat`: what time the notes are played for each specific measure
	- `dur`: how long the sounds will play for
	- `cfi`: cross fading in for sharper sounds
	- `cfo`: cross fading out for sharper sounds
	- `amp`: the amplitude of each pitch
	- `note`: the note being played
	- `ADSR`: refers to the level and time of each pitch
	- `vibrato`: a pulsing change of pitch in the sound

### Example
[Score File](additive-synthesizer.score)

[Audio File](additive-sythesizer.wav)

## Wavetable
Anthony Rodeman, component owner

Wavetable using unplugged electric guitar samples I recorded.

- Wave playback on demand from the sequencer
- Envelope generation (fade out, as noted on page)
- Pitch
- Attack and Sustain Waves (3 possible attacks, then 1 sustain wave)
- Table Cross-Fade (cross-fading between each phase of wave: attack, sustain, decay)

### Parameters:
	- `type`: either "harmonic", "pluck", or "strum" (default "strum") to select attack type

### Example
Horn Solo from *You Can't Always Get What You Want* by the Rolling Stones (from a transcription I did for MUS 178 in the Spring)

[Score File](wavetable-example.score)

[Audio File](wavetable-example.wav)


## XML Format
Extension: `.score`
A `score` head node which has attributes:
	- `bpm`: beats per minute
	- `beatspermeasure`: how many beats occur in a measure
The score node then has as many `instrument` notes as desired. Each `instrument` node has one attribute:
	- `instrument`: the name of the instrument to use.

"Chorus", "Flange", "NoiseGate", and "Limiter" options are effects descriptors. 
These instruments must appear once to use effects on sound instruments and one `note` node is used to input values through attributes. 
The settings for each effect can be seen under the Effects section. If no effects are used, the effect-named `instrument` tags are optional.

The AdditiveSynth, Wavetable, and ToneInstrument all can generate sound. Each can have a `chorus`, `flange`, `limiting`, and `noisegating` attribute, which corresponds to the use of that effect for child notes of the `instrument`. Accepted values are "0" (do not use effect) or "1" (use effect), with default of "0".

`note` nodes are always leaf nodes which are children of `instrument` nodes. For instruments that can generate sound, the following attributes must be defined:
	- `measure`: which measure the note is played in (1-indexed integer)
	- `beat`: which beat in the measure the note is played on (1-indexed floating point up to `beatspermeasure`, exclusive)
	- `dur`: the length in beats which the note is played for (floating point)
	- `note`: string which names the note in scientific pitch notation to be played (use 'b' for the flat symbol)
Each instrument may have more possible attributes. These attributes must be given on a per-`note` basis. Information about these attributes is found under the component sections.