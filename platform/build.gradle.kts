plugins {
    `java-library`
    `maven-publish`
}

val platform: String by project

repositories {
    mavenCentral()
}

dependencies {
}

tasks.jar {
    archiveFileName.set("$platform.jar")
}