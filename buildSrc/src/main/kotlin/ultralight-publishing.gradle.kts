plugins {
    `maven-publish`
}

publishing {
    repositories {
        maven("https://repo.repsy.io/nilsen84/maven") {
            credentials {
                username = System.getenv("REPSY_USERNAME")
                password = System.getenv("REPSY_TOKEN")
            }
        }
    }
}
