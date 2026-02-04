package it.unibo.iot03.com;

import java.util.Date;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import it.unibo.iot03.model.Data;
import it.unibo.iot03.model.Logic;

/*
 * Data Service as a vertx event-loop
 */
public class HttpServer extends AbstractVerticle {

	private int port;
	private final Logic logic;

	public HttpServer(final int port, Logic logic) {
		this.port = port;
		this.logic = logic;
	}

	@Override
	public void start() {
		Router router = Router.router(vertx);
		router.route().handler(BodyHandler.create());
		router.post("/api/data").handler(this::handleAddNewData);
		router.get("/api/data").handler(this::handleGetData);
		vertx
			.createHttpServer()
			.requestHandler(router)
			.listen(port);

		log("Service ready on port: " + port);
	}

	private void handleAddNewData(RoutingContext routingContext) {
		HttpServerResponse response = routingContext.response();
		// log("new msg "+routingContext.body().asString());
		JsonObject res = routingContext.body().asJsonObject();
		if (res == null) {
			sendError(400, response);
		} else {
			float value = res.getFloat("value");
			String place = res.getString("place");
			long time = System.currentTimeMillis();

			log("New value: " + value + " from " + place + " on " + new Date(time));
			response.setStatusCode(200).end();
		}
	}

	private void handleGetData(RoutingContext routingContext) {
		JsonArray arr = new JsonArray();
		for (Data p: this.logic.getHistory()) {
			JsonObject data = new JsonObject();
			data.put("timestamp", p.formattedTime());
			data.put("value", p.value());
			arr.add(data);
		}
		routingContext.response()
			.putHeader("content-type", "application/json")
			.end(arr.encodePrettily());
	}

	private void sendError(int statusCode, HttpServerResponse response) {
		response.setStatusCode(statusCode).end();
	}

	private void log(String msg) {
		System.out.println("[HTTP SERVER] " + msg);
	}

}