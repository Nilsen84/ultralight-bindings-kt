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
        }
    }
}