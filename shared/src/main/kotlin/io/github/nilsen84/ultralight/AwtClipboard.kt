package io.github.nilsen84.ultralight

import java.awt.Toolkit
import java.awt.datatransfer.DataFlavor
import java.awt.datatransfer.StringSelection

class AwtClipboard: UltralightClipboard {
    private val clipboard = Toolkit.getDefaultToolkit().systemClipboard

    override fun clear() {
        val stringSelection = StringSelection("")
        this.clipboard.setContents(stringSelection, null)
    }

    override fun read(): String {
        try {
            if (this.clipboard.isDataFlavorAvailable(DataFlavor.stringFlavor)) {
                return this.clipboard.getData(DataFlavor.stringFlavor) as String
            }
        } catch (e: Exception) {
            e.printStackTrace()
        }
        return ""
    }

    override fun write(text: String) {
        val stringSelection = StringSelection(text)
        this.clipboard.setContents(stringSelection, null)
    }
}