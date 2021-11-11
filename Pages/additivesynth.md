# Additive Synthesizer
Cameron Hurley, component owner
- Sinusoid playback on demand from the sequencer
- Definition of all harmonics
- Envelope generation
- Polyphony
- Sound cross-fading
- Vibrato OR glissando

## Parameters:
- `measure`: the time the notes are played
- `beat`: what time the notes are played for each specific measure
- `dur`: how long the sounds will play for
- `cfi`: cross fading in for sharper sounds
- `cfo`: cross fading out for sharper sounds
- `amp`: the amplitude of each pitch
- `note`: the note being played
- `ADSR`: refers to the level and time of each pitch
- `vibrato`: a pulsing change of pitch in the sound

## Example
[Score File](Scores/additive-synthesizer.score)

[Audio File](Scores/additive-sythesizer.wav)