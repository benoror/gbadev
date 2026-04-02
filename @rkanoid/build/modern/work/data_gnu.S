.section .rodata, "a", %progbits

.align 2
.global sprites
sprites:
    .incbin "bitmaps/sprites.raw"

.align 2
.global sprite_palette
sprite_palette:
    .incbin "palettes/sprites.pal"

.align 2
.global bg
bg:
    .incbin "bitmaps/bg.raw"

.align 2
.global bg_pal
bg_pal:
    .incbin "palettes/bg.pal"

.align 2
.global titulo
titulo:
    .incbin "bitmaps/titulo.raw"

.align 2
.global titulo_pal
titulo_pal:
    .incbin "palettes/titulo.pal"

.align 2
.global gameOver
gameOver:
    .incbin "bitmaps/gameover.raw"

.align 2
.global gameOver_pal
gameOver_pal:
    .incbin "palettes/gameover.pal"

.align 2
.global fin
fin:
    .incbin "bitmaps/fin.raw"

.align 2
.global fin_pal
fin_pal:
    .incbin "palettes/fin.pal"

.align 2
.global dream
dream:
    .incbin "bitmaps/dream.raw"

.align 2
.global dream_pal
dream_pal:
    .incbin "palettes/dream.pal"
