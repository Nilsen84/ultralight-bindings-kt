plugins {
    `java-library`
    `maven-publish`
}

val platform: String by project

tasks.processResources {
    from("src/main/natives") {
        into("io/github/nilsen84/ultralight/natives/$platform")
    }
}

repositories {
    mavenCentral()
}

dependencies {
}

tasks.jar {
    archiveFileName.set("$platform.jar")
}