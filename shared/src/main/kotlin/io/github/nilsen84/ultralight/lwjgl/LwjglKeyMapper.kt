package io.github.nilsen84.ultralight.lwjgl

import io.github.nilsen84.ultralight.UltralightKey

object LwjglKeyMapper {
    fun toUltralight(lwjgl: Int): UltralightKey {
        return when (lwjgl) {
            LwjglKey.KEY_ESCAPE -> UltralightKey.KEY_ESCAPE
            LwjglKey.KEY_TAB -> UltralightKey.KEY_TAB
            LwjglKey.KEY_LCONTROL -> UltralightKey.KEY_LCONTROL
            LwjglKey.KEY_RCONTROL -> UltralightKey.KEY_RCONTROL
            LwjglKey.KEY_LSHIFT -> UltralightKey.KEY_LSHIFT
            LwjglKey.KEY_RSHIFT -> UltralightKey.KEY_RSHIFT
            LwjglKey.KEY_LMENU -> UltralightKey.KEY_LMENU
            LwjglKey.KEY_RMENU -> UltralightKey.KEY_RMENU
            LwjglKey.KEY_LMETA -> UltralightKey.KEY_LWIN
            LwjglKey.KEY_RMETA -> UltralightKey.KEY_RWIN
            LwjglKey.KEY_SPACE -> UltralightKey.KEY_SPACE
            LwjglKey.KEY_RETURN -> UltralightKey.KEY_RETURN
            LwjglKey.KEY_BACK -> UltralightKey.KEY_BACK
            LwjglKey.KEY_CAPITAL -> UltralightKey.KEY_CAPITAL

            LwjglKey.KEY_1 -> UltralightKey.KEY_1
            LwjglKey.KEY_2 -> UltralightKey.KEY_2
            LwjglKey.KEY_3 -> UltralightKey.KEY_3
            LwjglKey.KEY_4 -> UltralightKey.KEY_4
            LwjglKey.KEY_5 -> UltralightKey.KEY_5
            LwjglKey.KEY_6 -> UltralightKey.KEY_6
            LwjglKey.KEY_7 -> UltralightKey.KEY_7
            LwjglKey.KEY_8 -> UltralightKey.KEY_8
            LwjglKey.KEY_9 -> UltralightKey.KEY_9
            LwjglKey.KEY_0 -> UltralightKey.KEY_0
            LwjglKey.KEY_A -> UltralightKey.KEY_A
            LwjglKey.KEY_B -> UltralightKey.KEY_B
            LwjglKey.KEY_C -> UltralightKey.KEY_C
            LwjglKey.KEY_D -> UltralightKey.KEY_D
            LwjglKey.KEY_E -> UltralightKey.KEY_E
            LwjglKey.KEY_F -> UltralightKey.KEY_F
            LwjglKey.KEY_G -> UltralightKey.KEY_G
            LwjglKey.KEY_H -> UltralightKey.KEY_H
            LwjglKey.KEY_I -> UltralightKey.KEY_I
            LwjglKey.KEY_J -> UltralightKey.KEY_J
            LwjglKey.KEY_K -> UltralightKey.KEY_K
            LwjglKey.KEY_L -> UltralightKey.KEY_L
            LwjglKey.KEY_M -> UltralightKey.KEY_M
            LwjglKey.KEY_N -> UltralightKey.KEY_N
            LwjglKey.KEY_O -> UltralightKey.KEY_O
            LwjglKey.KEY_P -> UltralightKey.KEY_P
            LwjglKey.KEY_Q -> UltralightKey.KEY_Q
            LwjglKey.KEY_R -> UltralightKey.KEY_R
            LwjglKey.KEY_S -> UltralightKey.KEY_S
            LwjglKey.KEY_T -> UltralightKey.KEY_T
            LwjglKey.KEY_U -> UltralightKey.KEY_U
            LwjglKey.KEY_V -> UltralightKey.KEY_V
            LwjglKey.KEY_W -> UltralightKey.KEY_W
            LwjglKey.KEY_X -> UltralightKey.KEY_X
            LwjglKey.KEY_Y -> UltralightKey.KEY_Y
            LwjglKey.KEY_Z -> UltralightKey.KEY_Z

            LwjglKey.KEY_MINUS -> UltralightKey.KEY_OEM_MINUS
            LwjglKey.KEY_EQUALS -> UltralightKey.KEY_OEM_PLUS
            LwjglKey.KEY_LBRACKET -> UltralightKey.KEY_OEM_4
            LwjglKey.KEY_RBRACKET -> UltralightKey.KEY_OEM_6
            LwjglKey.KEY_SEMICOLON -> UltralightKey.KEY_OEM_1
            LwjglKey.KEY_APOSTROPHE -> UltralightKey.KEY_OEM_7
            LwjglKey.KEY_GRAVE -> UltralightKey.KEY_OEM_3
            LwjglKey.KEY_BACKSLASH -> UltralightKey.KEY_OEM_5
            LwjglKey.KEY_COMMA -> UltralightKey.KEY_OEM_COMMA
            LwjglKey.KEY_PERIOD -> UltralightKey.KEY_OEM_PERIOD
            LwjglKey.KEY_SLASH -> UltralightKey.KEY_OEM_2

            LwjglKey.KEY_F1 -> UltralightKey.KEY_F1
            LwjglKey.KEY_F2 -> UltralightKey.KEY_F2
            LwjglKey.KEY_F3 -> UltralightKey.KEY_F3
            LwjglKey.KEY_F4 -> UltralightKey.KEY_F4
            LwjglKey.KEY_F5 -> UltralightKey.KEY_F5
            LwjglKey.KEY_F6 -> UltralightKey.KEY_F6
            LwjglKey.KEY_F7 -> UltralightKey.KEY_F7
            LwjglKey.KEY_F8 -> UltralightKey.KEY_F8
            LwjglKey.KEY_F9 -> UltralightKey.KEY_F9
            LwjglKey.KEY_F10 -> UltralightKey.KEY_F10
            LwjglKey.KEY_F11 -> UltralightKey.KEY_F11
            LwjglKey.KEY_F12 -> UltralightKey.KEY_F12
            LwjglKey.KEY_F13 -> UltralightKey.KEY_F13
            LwjglKey.KEY_F14 -> UltralightKey.KEY_F14
            LwjglKey.KEY_F15 -> UltralightKey.KEY_F15
            LwjglKey.KEY_F16 -> UltralightKey.KEY_F16
            LwjglKey.KEY_F17 -> UltralightKey.KEY_F17
            LwjglKey.KEY_F18 -> UltralightKey.KEY_F18
            LwjglKey.KEY_F19 -> UltralightKey.KEY_F19

            LwjglKey.KEY_NUMLOCK -> UltralightKey.KEY_NUMLOCK
            LwjglKey.KEY_NUMPAD0 -> UltralightKey.KEY_NUMPAD0
            LwjglKey.KEY_NUMPAD1 -> UltralightKey.KEY_NUMPAD1
            LwjglKey.KEY_NUMPAD2 -> UltralightKey.KEY_NUMPAD2
            LwjglKey.KEY_NUMPAD3 -> UltralightKey.KEY_NUMPAD3
            LwjglKey.KEY_NUMPAD4 -> UltralightKey.KEY_NUMPAD4
            LwjglKey.KEY_NUMPAD5 -> UltralightKey.KEY_NUMPAD5
            LwjglKey.KEY_NUMPAD6 -> UltralightKey.KEY_NUMPAD6
            LwjglKey.KEY_NUMPAD7 -> UltralightKey.KEY_NUMPAD7
            LwjglKey.KEY_NUMPAD8 -> UltralightKey.KEY_NUMPAD8
            LwjglKey.KEY_NUMPAD9 -> UltralightKey.KEY_NUMPAD9
            LwjglKey.KEY_ADD -> UltralightKey.KEY_ADD
            LwjglKey.KEY_SUBTRACT -> UltralightKey.KEY_SUBTRACT
            LwjglKey.KEY_MULTIPLY -> UltralightKey.KEY_MULTIPLY
            LwjglKey.KEY_DIVIDE -> UltralightKey.KEY_DIVIDE
            LwjglKey.KEY_DECIMAL -> UltralightKey.KEY_DECIMAL
            LwjglKey.KEY_NUMPADENTER -> UltralightKey.KEY_RETURN
            LwjglKey.KEY_NUMPADCOMMA -> UltralightKey.KEY_SEPARATOR

            LwjglKey.KEY_UP -> UltralightKey.KEY_UP
            LwjglKey.KEY_DOWN -> UltralightKey.KEY_DOWN
            LwjglKey.KEY_LEFT -> UltralightKey.KEY_LEFT
            LwjglKey.KEY_RIGHT -> UltralightKey.KEY_RIGHT
            LwjglKey.KEY_INSERT -> UltralightKey.KEY_INSERT
            LwjglKey.KEY_DELETE -> UltralightKey.KEY_DELETE
            LwjglKey.KEY_HOME -> UltralightKey.KEY_HOME
            LwjglKey.KEY_END -> UltralightKey.KEY_END
            LwjglKey.KEY_PRIOR -> UltralightKey.KEY_PRIOR
            LwjglKey.KEY_NEXT -> UltralightKey.KEY_NEXT

            LwjglKey.KEY_SCROLL -> UltralightKey.KEY_SCROLL
            LwjglKey.KEY_PAUSE -> UltralightKey.KEY_PAUSE
            LwjglKey.KEY_SYSRQ -> UltralightKey.KEY_SNAPSHOT
            LwjglKey.KEY_APPS -> UltralightKey.KEY_APPS
            LwjglKey.KEY_SLEEP -> UltralightKey.KEY_SLEEP
            LwjglKey.KEY_CLEAR -> UltralightKey.KEY_CLEAR

            LwjglKey.KEY_KANA -> UltralightKey.KEY_KANA
            LwjglKey.KEY_KANJI -> UltralightKey.KEY_KANJI
            LwjglKey.KEY_CONVERT -> UltralightKey.KEY_CONVERT
            LwjglKey.KEY_NOCONVERT -> UltralightKey.KEY_NONCONVERT

            LwjglKey.KEY_YEN,
            LwjglKey.KEY_NUMPADEQUALS,
            LwjglKey.KEY_CIRCUMFLEX,
            LwjglKey.KEY_AT,
            LwjglKey.KEY_COLON,
            LwjglKey.KEY_UNDERLINE,
            LwjglKey.KEY_STOP,
            LwjglKey.KEY_AX,
            LwjglKey.KEY_UNLABELED,
            LwjglKey.KEY_SECTION,
            LwjglKey.KEY_FUNCTION,
            LwjglKey.KEY_POWER,
            LwjglKey.KEY_NONE -> UltralightKey.KEY_UNKNOWN

            else -> UltralightKey.KEY_UNKNOWN
        }
    }
}

private object LwjglKey {
    const val KEY_NONE: Int = 0x00
    const val KEY_ESCAPE: Int = 0x01
    const val KEY_1: Int = 0x02
    const val KEY_2: Int = 0x03
    const val KEY_3: Int = 0x04
    const val KEY_4: Int = 0x05
    const val KEY_5: Int = 0x06
    const val KEY_6: Int = 0x07
    const val KEY_7: Int = 0x08
    const val KEY_8: Int = 0x09
    const val KEY_9: Int = 0x0A
    const val KEY_0: Int = 0x0B
    const val KEY_MINUS: Int = 0x0C 
    const val KEY_EQUALS: Int = 0x0D
    const val KEY_BACK: Int = 0x0E 
    const val KEY_TAB: Int = 0x0F
    const val KEY_Q: Int = 0x10
    const val KEY_W: Int = 0x11
    const val KEY_E: Int = 0x12
    const val KEY_R: Int = 0x13
    const val KEY_T: Int = 0x14
    const val KEY_Y: Int = 0x15
    const val KEY_U: Int = 0x16
    const val KEY_I: Int = 0x17
    const val KEY_O: Int = 0x18
    const val KEY_P: Int = 0x19
    const val KEY_LBRACKET: Int = 0x1A
    const val KEY_RBRACKET: Int = 0x1B
    const val KEY_RETURN: Int = 0x1C 
    const val KEY_LCONTROL: Int = 0x1D
    const val KEY_A: Int = 0x1E
    const val KEY_S: Int = 0x1F
    const val KEY_D: Int = 0x20
    const val KEY_F: Int = 0x21
    const val KEY_G: Int = 0x22
    const val KEY_H: Int = 0x23
    const val KEY_J: Int = 0x24
    const val KEY_K: Int = 0x25
    const val KEY_L: Int = 0x26
    const val KEY_SEMICOLON: Int = 0x27
    const val KEY_APOSTROPHE: Int = 0x28
    const val KEY_GRAVE: Int = 0x29 
    const val KEY_LSHIFT: Int = 0x2A
    const val KEY_BACKSLASH: Int = 0x2B
    const val KEY_Z: Int = 0x2C
    const val KEY_X: Int = 0x2D
    const val KEY_C: Int = 0x2E
    const val KEY_V: Int = 0x2F
    const val KEY_B: Int = 0x30
    const val KEY_N: Int = 0x31
    const val KEY_M: Int = 0x32
    const val KEY_COMMA: Int = 0x33
    const val KEY_PERIOD: Int = 0x34 
    const val KEY_SLASH: Int = 0x35 
    const val KEY_RSHIFT: Int = 0x36
    const val KEY_MULTIPLY: Int = 0x37 
    const val KEY_LMENU: Int = 0x38 
    const val KEY_SPACE: Int = 0x39
    const val KEY_CAPITAL: Int = 0x3A
    const val KEY_F1: Int = 0x3B
    const val KEY_F2: Int = 0x3C
    const val KEY_F3: Int = 0x3D
    const val KEY_F4: Int = 0x3E
    const val KEY_F5: Int = 0x3F
    const val KEY_F6: Int = 0x40
    const val KEY_F7: Int = 0x41
    const val KEY_F8: Int = 0x42
    const val KEY_F9: Int = 0x43
    const val KEY_F10: Int = 0x44
    const val KEY_NUMLOCK: Int = 0x45
    const val KEY_SCROLL: Int = 0x46 
    const val KEY_NUMPAD7: Int = 0x47
    const val KEY_NUMPAD8: Int = 0x48
    const val KEY_NUMPAD9: Int = 0x49
    const val KEY_SUBTRACT: Int = 0x4A 
    const val KEY_NUMPAD4: Int = 0x4B
    const val KEY_NUMPAD5: Int = 0x4C
    const val KEY_NUMPAD6: Int = 0x4D
    const val KEY_ADD: Int = 0x4E 
    const val KEY_NUMPAD1: Int = 0x4F
    const val KEY_NUMPAD2: Int = 0x50
    const val KEY_NUMPAD3: Int = 0x51
    const val KEY_NUMPAD0: Int = 0x52
    const val KEY_DECIMAL: Int = 0x53 
    const val KEY_F11: Int = 0x57
    const val KEY_F12: Int = 0x58
    const val KEY_F13: Int = 0x64 
    const val KEY_F14: Int = 0x65 
    const val KEY_F15: Int = 0x66 
    const val KEY_F16: Int = 0x67 
    const val KEY_F17: Int = 0x68
    const val KEY_F18: Int = 0x69
    const val KEY_KANA: Int = 0x70 
    const val KEY_F19: Int = 0x71 
    const val KEY_CONVERT: Int = 0x79 
    const val KEY_NOCONVERT: Int = 0x7B 
    const val KEY_YEN: Int = 0x7D 
    const val KEY_NUMPADEQUALS: Int = 0x8D 
    const val KEY_CIRCUMFLEX: Int = 0x90 
    const val KEY_AT: Int = 0x91 
    const val KEY_COLON: Int = 0x92 
    const val KEY_UNDERLINE: Int = 0x93 
    const val KEY_KANJI: Int = 0x94 
    const val KEY_STOP: Int = 0x95 
    const val KEY_AX: Int = 0x96 
    const val KEY_UNLABELED: Int = 0x97 
    const val KEY_NUMPADENTER: Int = 0x9C 
    const val KEY_RCONTROL: Int = 0x9D
    const val KEY_SECTION: Int = 0xA7 
    const val KEY_NUMPADCOMMA: Int = 0xB3 
    const val KEY_DIVIDE: Int = 0xB5 
    const val KEY_SYSRQ: Int = 0xB7
    const val KEY_RMENU: Int = 0xB8 
    const val KEY_FUNCTION: Int = 0xC4 
    const val KEY_PAUSE: Int = 0xC5 
    const val KEY_HOME: Int = 0xC7 
    const val KEY_UP: Int = 0xC8 
    const val KEY_PRIOR: Int = 0xC9 
    const val KEY_LEFT: Int = 0xCB 
    const val KEY_RIGHT: Int = 0xCD 
    const val KEY_END: Int = 0xCF 
    const val KEY_DOWN: Int = 0xD0 
    const val KEY_NEXT: Int = 0xD1 
    const val KEY_INSERT: Int = 0xD2 
    const val KEY_DELETE: Int = 0xD3 
    const val KEY_CLEAR: Int = 0xDA 
    const val KEY_LMETA: Int = 0xDB 
    const val KEY_RMETA: Int = 0xDC
    const val KEY_APPS: Int = 0xDD
    const val KEY_POWER: Int = 0xDE
    const val KEY_SLEEP: Int = 0xDF
}