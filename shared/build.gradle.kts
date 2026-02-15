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

            val externalPlatformJars = providers.gradleProperty("externalPlatformJars")
            if (externalPlatformJars.isPresent) {
                val jarsDir = rootProject.layout.projectDirectory.dir(externalPlatformJars)
                val jarsTree = fileTree(jarsDir) { include("*.jar") }
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