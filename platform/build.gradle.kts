plugins {
    `java-library`
    `maven-publish`
    `ultralight-publishing`
}

val platform: String by project

repositories {
    mavenCentral()
}

dependencies {
}

tasks.jar {
    archiveClassifier.set(platform)
}

publishing {
    publications {
        create<MavenPublication>("maven") {
            artifactId = rootProject.name
            from(components["java"])
        }
    }
}