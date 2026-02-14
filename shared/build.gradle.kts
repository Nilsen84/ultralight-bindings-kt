plugins {
    kotlin("jvm") version "2.3.10"
    `java-library`
    `maven-publish`
    `ultralight-publishing`
}

repositories {
    mavenCentral()
}

dependencies {

}

publishing {
    publications {
        create<MavenPublication>("maven") {
            from(components["java"])
            artifactId = rootProject.name

            val externalPlatformJarsProvider = providers.gradleProperty("externalPlatformJars")
            if (externalPlatformJarsProvider.isPresent) {
                val dirPath = externalPlatformJarsProvider.get()
                val jarsDir = rootProject.layout.projectDirectory.dir(dirPath)
                val jarsTree = rootProject.fileTree(jarsDir) { include("*.jar") }
                jarsTree.files.forEach { jarFile ->
                    artifact(jarFile) {
                        classifier = jarFile.nameWithoutExtension
                    }
                }
            }
        }
    }
}

kotlin.jvmToolchain(8)